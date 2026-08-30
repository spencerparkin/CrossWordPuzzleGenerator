#include "CrossWord/PuzzleGenerator.h"
#include "CrossWord/PuzzleMatrix.h"
#include "CrossWord/WordBank.h"

using namespace CrossWord;

PuzzleGenerator::PuzzleGenerator()
{
}

/*virtual*/ PuzzleGenerator::~PuzzleGenerator()
{
}

bool PuzzleGenerator::Generate(PuzzleMatrix* puzzleMatrix, WordBank* workBank)
{
	return false;
}