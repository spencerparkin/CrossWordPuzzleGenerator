#include "CrossWord/PuzzleGenerator.h"
#include "CrossWord/PuzzleMatrix.h"
#include "CrossWord/WordBank.h"
#include "CrossWord/Random.h"

using namespace CrossWord;

PuzzleGenerator::PuzzleGenerator()
{
}

/*virtual*/ PuzzleGenerator::~PuzzleGenerator()
{
}

bool PuzzleGenerator::Generate(PuzzleMatrix* puzzleMatrix, WordBank* workBank, Random* random, int minWordLength, int maxWordLength, bool symmetric)
{
	int numRows = puzzleMatrix->GetNumRows();
	int numCols = puzzleMatrix->GetNumCols();

	for (int row = 0; row < numRows; row++)
		for (int col = 0; col < numCols; col++)
			puzzleMatrix->SetLetter(row, col, CROSSWORD_LETTER_UNKNOWN);

	while (true)
	{
		std::vector<WordLocation> wordLocationArray;
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

	// STPTODO: Okay, next step is to fit the words in....

	return true;
}