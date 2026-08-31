#pragma once

#include <string>
#include <stdio.h>
#include "CrossWord/PuzzleMatrix.h"

#ifdef CROSSWORD_ENABLE_LOG
#define CROSSWORD_LOG			printf
#else
#define CROSSWORD_LOG
#endif

namespace CrossWord
{
	class WordBank;
	class Random;

	/**
	 * These can generate valid crossword puzzle matrices.  The plan is
	 * to do this in two steps.  First, generate the shape of the puzzle.
	 * This means deciding which elements of the matrix are to hold a letter,
	 * and which are to always be blank.  This will determine the number of
	 * words in the puzzle as well as each word's length.  Seconds, a brute-
	 * force recursive algorithm tries to randomly yet systematically fit
	 * words into the matrix.  This algorithm will be correct in that it
	 * will find an answer, if it exists, but whether it terminates in a
	 * reasonable amount of time remains to be seen.  It's recursive for
	 * the purpose of backtracking.  If we reach an empass, then we have
	 * to return failure to the caller so that they can make a different
	 * choice.
	 */
	class PuzzleGenerator
	{
	public:
		PuzzleGenerator();
		virtual ~PuzzleGenerator();

		bool Generate(PuzzleMatrix* puzzleMatrix, WordBank* wordBank, Random* random, int minWordLength, int maxWordLength, bool symmetric, std::vector<WordLocation>& wordLocationArray);

	private:

		bool FitWords(PuzzleMatrix* puzzleMatrix, WordBank* wordBank, Random* random, const std::vector<WordLocation>& wordLocationArray);
	};
}