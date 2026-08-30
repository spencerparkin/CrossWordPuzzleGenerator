#include "CrossWord/PuzzleGenerator.h"
#include "CrossWord/PuzzleMatrix.h"
#include "CrossWord/WordBank.h"
#include "CrossWord/Random.h"
#include <iostream>

int main(int argc, char** argv)
{
	CrossWord::WordBank wordBank;

	std::cout << "Loading words...\n";

	if (!wordBank.Load(R"(D:\git_repos\CrossWordPuzzleGenerator\Test\Words.txt)"))
		return 1;

	CrossWord::PuzzleMatrix puzzleMatrix;
	puzzleMatrix.SetSize(16, 16);

	CrossWord::PuzzleGenerator puzzleGenerator;
	CrossWord::Random random;

	random.SetSeed(123);

	std::cout << "Generating puzzle...\n";

	if (!puzzleGenerator.Generate(&puzzleMatrix, &wordBank, &random, 3, 6, true))
		return 1;

	std::string puzzleStr = puzzleMatrix.Print();

	std::cout << puzzleStr;

	return 0;
}