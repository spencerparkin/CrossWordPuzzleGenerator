#include "CrossWord/PuzzleGenerator.h"
#include "CrossWord/PuzzleMatrix.h"
#include "CrossWord/WordBank.h"
#include "CrossWord/Random.h"
#include <iostream>
#include <assert.h>

int main(int argc, char** argv)
{
	CrossWord::WordBank wordBank;

	std::cout << "Loading words...\n";

	if (!wordBank.Load(R"(D:\git_repos\CrossWordPuzzleGenerator\Test\Words.txt)"))
		return 1;

	CrossWord::PuzzleMatrix puzzleMatrix;
	puzzleMatrix.SetSize(20, 20);

	CrossWord::PuzzleGenerator puzzleGenerator;
	CrossWord::Random random;

	random.SetSeed(4321);

	std::cout << "Generating puzzle...\n";

	std::vector<CrossWord::WordLocation> wordLocationArray;

	if (!puzzleGenerator.Generate(&puzzleMatrix, &wordBank, &random, 3, 9, true, wordLocationArray))
		return 1;

	std::cout << puzzleMatrix.Print() << "\n";

	for (const CrossWord::WordLocation& wordLocation : wordLocationArray)
	{
		std::string word = puzzleMatrix.GetWordAt(wordLocation);
		std::cout << "(" << wordLocation.location.row << ", " << wordLocation.location.col << ") ";
		std::cout << ((wordLocation.orientation == CrossWord::WordOrientation::ACROSS) ? "across" : "down");
		std::cout << " - " << word << "\n";

		assert(wordBank.IsWord(word));
	}

	return 0;
}