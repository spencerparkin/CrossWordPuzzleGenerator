#include "CrossWord/PuzzleGenerator.h"
#include "CrossWord/PuzzleMatrix.h"
#include "CrossWord/WordBank.h"
#include "CrossWord/Random.h"
#include <assert.h>

using namespace CrossWord;

PuzzleGenerator::PuzzleGenerator()
{
}

/*virtual*/ PuzzleGenerator::~PuzzleGenerator()
{
}

bool PuzzleGenerator::Generate(PuzzleMatrix* puzzleMatrix, WordBank* wordBank, Random* random, int minWordLength, int maxWordLength, bool symmetric, std::vector<WordLocation>& wordLocationArray)
{
	int numRows = puzzleMatrix->GetNumRows();
	int numCols = puzzleMatrix->GetNumCols();

	for (int row = 0; row < numRows; row++)
		for (int col = 0; col < numCols; col++)
			puzzleMatrix->SetLetter(row, col, CROSSWORD_LETTER_UNKNOWN);

	while (true)
	{
		puzzleMatrix->GetAllWordLocations(wordLocationArray);

		bool wordBoundsMet = true;

		for (const WordLocation& wordLocation : wordLocationArray)
		{
			if (wordLocation.length < minWordLength || wordLocation.length > maxWordLength)
			{
				wordBoundsMet = false;
				break;
			}
		}

		if (wordBoundsMet)
			break;

		std::vector<Location> locationArray;
		puzzleMatrix->GetAllLocationsWithLetter(locationArray, CROSSWORD_LETTER_UNKNOWN);

		random->Shuffle(locationArray.data(), (uint32_t)locationArray.size());

		bool blankFound = false;

		for (int i = 0; i < (int)locationArray.size(); i++)
		{
			const Location& location = locationArray[i];

			puzzleMatrix->SetLetter(location, CROSSWORD_ALWAYS_BLANK);

			if (symmetric)
				puzzleMatrix->SetLetter(puzzleMatrix->GetSymmetricLocation(location), CROSSWORD_ALWAYS_BLANK);

			puzzleMatrix->GetAllWordLocations(wordLocationArray);

			bool keepChange = true;

			for (const WordLocation& wordLocation : wordLocationArray)
			{
				if (wordLocation.length < minWordLength)
				{
					keepChange = false;
					break;
				}
			}

			if (keepChange && puzzleMatrix->HasHole())
				keepChange = false;

			// STPTODO: Might want to add a condition here that no word is in isolation.
			//          That is, every word should have at least one other word overlapping it.

			if (keepChange)
			{
				blankFound = true;
				break;
			}

			puzzleMatrix->SetLetter(location, CROSSWORD_LETTER_UNKNOWN);

			if (symmetric)
				puzzleMatrix->SetLetter(puzzleMatrix->GetSymmetricLocation(location), CROSSWORD_LETTER_UNKNOWN);
		}

		if (!blankFound)
			return false;
	}

	return this->FitWords(puzzleMatrix, wordBank, random, wordLocationArray);
}

bool PuzzleGenerator::FitWords(PuzzleMatrix* puzzleMatrix, WordBank* wordBank, Random* random, const std::vector<WordLocation>& wordLocationArray)
{
	std::vector<WordLocation> completedWordLocationArray;
	std::vector<WordLocation> partialWordLocationArray;
	std::vector<WordLocation> emptyWordLocationArray;

	for (const WordLocation& wordLocation : wordLocationArray)
	{
		int numLetters = 0;
		for (int i = 0; i < wordLocation.length; i++)
		{
			unsigned char letter = puzzleMatrix->GetLetter(wordLocation.GetLocationAt(i));
			if (letter != CROSSWORD_LETTER_UNKNOWN)
				numLetters++;
		}

		if (numLetters == 0)
			emptyWordLocationArray.push_back(wordLocation);
		else if (numLetters == wordLocation.length)
			completedWordLocationArray.push_back(wordLocation);
		else
			partialWordLocationArray.push_back(wordLocation);
	}

	// We have to make sure that every completed word is actually a word.
	// This is because some words get completed as a result of trying
	// to complete other words.
	for (const WordLocation& wordLocation : completedWordLocationArray)
	{
		std::string word = puzzleMatrix->GetWordAt(wordLocation);
		if (!wordBank->IsWord(word))
			return false;
	}

	if (completedWordLocationArray.size() == wordLocationArray.size())
		return true;

#ifdef CROSSWORD_LOG_GENERATOR
	printf("%s\n\n", puzzleMatrix->Print().c_str());
#endif

	WordLocation wordLocation;
	std::vector<Stamp> stampArray;
	WordBank::WordProfile wordProfile;

	// We want to try to fit partial words before empty ones, because if things
	// aren't fitting, we need to know sooner rather than later.
	if (partialWordLocationArray.size() > 0)
	{
		// We want to choose the partial word location with the smallest associated set of possabilities.
		// This is not just to reduce our branch factor, but also, if the smallest set has size zero,
		// then we know that it doesn't make sense to follow the tree down any further at this point.
		// We could try some other partial word that _does_ have possibilities, but if any current
		// partial word has _no_ possibilities, then we'd be trying in vain.
		int leastWordCount = std::numeric_limits<int>::max();
		for (const WordLocation& partialWordLocation : partialWordLocationArray)
		{
			this->GenerateWordProfile(puzzleMatrix, partialWordLocation, wordProfile, stampArray);
			const std::vector<std::string>* wordArray = wordBank->GetAllWordsOfLengthWithProfile(partialWordLocation.length, wordProfile);
			if ((int)wordArray->size() < leastWordCount)
			{
				leastWordCount = (int)wordArray->size();
				wordLocation = partialWordLocation;
				if (leastWordCount == 0)
					return false;
			}
		}

		assert(leastWordCount != std::numeric_limits<int>::max());
	}
	else if (emptyWordLocationArray.size() > 0)
		wordLocation = emptyWordLocationArray[0];
	else
	{
		assert(false);
		return false;
	}

#ifdef CROSSWORD_LOG_GENERATOR
	printf("Trying to determine (%d, %d) %s\n\n", wordLocation.location.row, wordLocation.location.col, (wordLocation.orientation == WordOrientation::ACROSS ? "across" : "down"));
#endif

	this->GenerateWordProfile(puzzleMatrix, wordLocation, wordProfile, stampArray);

	const std::vector<std::string>* wordArray = wordBank->GetAllWordsOfLengthWithProfile(wordLocation.length, wordProfile);
	assert(wordArray && wordArray->size() > 0);

	std::vector<int> permutation;
	random->MakeRandomPermutation(permutation, (int)wordArray->size());

	for (int i : permutation)
	{
		const std::string& word = (*wordArray)[i];

		for (const Stamp& stamp : stampArray)
			puzzleMatrix->SetLetter(stamp.location, word[stamp.i]);

		if (this->FitWords(puzzleMatrix, wordBank, random, wordLocationArray))
			return true;

		for (const Stamp& stamp : stampArray)
			puzzleMatrix->SetLetter(stamp.location, CROSSWORD_LETTER_UNKNOWN);
	}
	
	return false;
}

void PuzzleGenerator::GenerateWordProfile(PuzzleMatrix* puzzleMatrix, const WordLocation& wordLocation, WordBank::WordProfile& wordProfile, std::vector<Stamp>& stampArray)
{
	wordProfile.Clear();
	stampArray.clear();

	for (int i = 0; i < wordLocation.length; i++)
	{
		Stamp stamp;
		stamp.i = i;
		stamp.location = wordLocation.GetLocationAt(i);
		unsigned char letter = puzzleMatrix->GetLetter(stamp.location);
		if (letter != CROSSWORD_LETTER_UNKNOWN)
			wordProfile.AddCharacteristic(i, letter);
		else
			stampArray.push_back(stamp);
	}
}