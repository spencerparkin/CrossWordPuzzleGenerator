#include "CrossWord/PuzzleMatrix.h"
#include <assert.h>

using namespace CrossWord;

PuzzleMatrix::PuzzleMatrix()
{
	this->numRows = 0;
	this->numCols = 0;
	this->matrix = nullptr;
}

/*virtual*/ PuzzleMatrix::~PuzzleMatrix()
{
	this->SetSize(0, 0);
}

void PuzzleMatrix::SetSize(int numRows, int numCols)
{
	for (int row = 0; row < this->numRows; row++)
		delete[] this->matrix[row];
	delete[] this->matrix;
	this->matrix = nullptr;

	this->numRows = numRows;
	this->numCols = numCols;

	if (this->numRows > 0 && this->numCols > 0)
	{
		this->matrix = new char* [this->numRows];
		for (int row = 0; row < this->numRows; row++)
		{
			this->matrix[row] = new char[this->numCols];
			for (int col = 0; col < this->numCols; col++)
				this->matrix[row][col] = CROSSWORD_ALWAYS_BLANK;
		}
	}
}

int PuzzleMatrix::GetNumRows() const
{
	return this->numRows;
}

int PuzzleMatrix::GetNumCols() const
{
	return this->numCols;
}

char PuzzleMatrix::GetLetter(int row, int col) const
{
	assert(0 <= row && row <= this->numRows);
	assert(0 <= col && col <= this->numCols);

	return this->matrix[row][col];
}

void PuzzleMatrix::SetLetter(int row, int col, char letter)
{
	assert(0 <= row && row <= this->numRows);
	assert(0 <= col && col <= this->numCols);

	this->matrix[row][col] = letter;
}

std::string PuzzleMatrix::Print() const
{
	return "";
}