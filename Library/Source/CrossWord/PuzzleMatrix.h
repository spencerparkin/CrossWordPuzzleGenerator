#pragma once

#include <string>

#define CROSSWORD_ALWAYS_BLANK		0xFF
#define CROSSWORD_LETTER_UNKNOWN	0x00

namespace CrossWord
{
	/**
	 * 
	 */
	class PuzzleMatrix
	{
	public:
		PuzzleMatrix();
		virtual ~PuzzleMatrix();

		void SetSize(int numRows, int numCols);
		int GetNumRows() const;
		int GetNumCols() const;

		char GetLetter(int row, int col) const;
		void SetLetter(int row, int col, char letter);

		std::string Print() const;

	private:
		char** matrix;
		int numRows;
		int numCols;
	};
}