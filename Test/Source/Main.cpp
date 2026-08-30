#include "CrossWord/PuzzleGenerator.h"
#include "CrossWord/PuzzleMatrix.h"
#include "CrossWord/WordBank.h"
#include <iostream>

int main(int argc, char** argv)
{
	CrossWord::WordBank wordBank;

	if (!wordBank.Load(R"(D:\git_repos\CrossWordPuzzleGenerator\Test\Words.txt)"))
		return 1;

	CrossWord::PuzzleMatrix puzzleMatrix;
	puzzleMatrix.SetSize(16, 16);

	CrossWord::PuzzleGenerator puzzleGenerator;

	if (!puzzleGenerator.Generate(&puzzleMatrix, &wordBank))
		return 1;

	std::string puzzleStr = puzzleMatrix.Print();

	std::cout << puzzleStr;

	return 0;
}