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

	// We want to try to fit partial words before empty ones, because if things
	// aren't fitting, we need to know sooner rather than later.
	WordLocation wordLocation;
	if (partialWordLocationArray.size() > 0)
		wordLocation = partialWordLocationArray[0];
	else if (emptyWordLocationArray.size() > 0)
		wordLocation = emptyWordLocationArray[0];
	else
		return false;

	struct Stamp
	{
		Location location;
		int i;
	};

	std::vector<Stamp> stampArray;
	WordBank::WordProfile wordProfile;

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

	const std::vector<std::string>* wordArray = wordBank->GetAllWordsOfLengthWithProfile(wordLocation.length, wordProfile);
	if (!wordArray)
	{
		assert(false);
		return false;
	}

	if (wordArray->size() == 0)
		return false;

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