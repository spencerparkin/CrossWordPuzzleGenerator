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
		this->matrix = new unsigned char* [this->numRows];
		for (int row = 0; row < this->numRows; row++)
		{
			this->matrix[row] = new unsigned char[this->numCols];
			for (int col = 0; col < this->numCols; col++)
				this->matrix[row][col] = CROSSWORD_ALWAYS_BLANK;
		}
	}
}

void PuzzleMatrix::Copy(const PuzzleMatrix& puzzleMatrix, bool copyLetters)
{
	this->SetSize(puzzleMatrix.numRows, puzzleMatrix.numCols);

	for (int row = 0; row < this->numRows; row++)
	{
		for (int col = 0; col < this->numCols; col++)
		{
			unsigned char letter = puzzleMatrix.matrix[row][col];

			if (letter != CROSSWORD_ALWAYS_BLANK)
				this->matrix[row][col] = copyLetters ? letter : CROSSWORD_LETTER_UNKNOWN;
			else
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

bool PuzzleMatrix::IsValidLocation(int row, int col) const
{
	if (row < 0 || row >= this->numRows)
		return false;

	if (col < 0 || col >= this->numCols)
		return false;

	return true;
}

bool PuzzleMatrix::IsValidLocation(const Location& location) const
{
	return this->IsValidLocation(location.row, location.col);
}

unsigned char PuzzleMatrix::GetLetter(int row, int col) const
{
	assert(0 <= row && row < this->numRows);
	assert(0 <= col && col < this->numCols);

	return this->matrix[row][col];
}

unsigned char PuzzleMatrix::GetLetter(const Location& location) const
{
	return this->GetLetter(location.row, location.col);
}

void PuzzleMatrix::SetLetter(int row, int col, unsigned char letter)
{
	assert(0 <= row && row < this->numRows);
	assert(0 <= col && col < this->numCols);

	this->matrix[row][col] = letter;
}

void PuzzleMatrix::SetLetter(const Location& location, unsigned char letter)
{
	this->matrix[location.row][location.col] = letter;
}

Location PuzzleMatrix::GetSymmetricLocation(const Location& location) const
{
	Location symmetricLocation;
	symmetricLocation.row = this->numRows - 1 - location.row;
	symmetricLocation.col = this->numCols - 1 - location.col;
	return symmetricLocation;
}

std::string PuzzleMatrix::Print() const
{
	std::string puzzleStr;

	for (int row = 0; row < this->numRows; row++)
	{
		for (int col = 0; col < this->numCols; col++)
		{
			unsigned char letter = this->matrix[row][col];
			puzzleStr += letter;
		}

		puzzleStr += "\n";
	}

	return puzzleStr;
}

void PuzzleMatrix::GetAllWordLocations(std::vector<WordLocation>& wordLocationArray) const
{
	wordLocationArray.clear();

	for (int row = 0; row < this->numRows; row++)
	{
		for (int col = 0; col < this->numCols; col++)
		{
			if ((col == 0 || this->matrix[row][col - 1] == CROSSWORD_ALWAYS_BLANK) && this->matrix[row][col] == CROSSWORD_LETTER_UNKNOWN)
			{
				WordLocation wordLocation;
				wordLocation.location.row = row;
				wordLocation.location.col = col;
				wordLocation.orientation = WordOrientation::ACROSS;
				wordLocation.length = 0;

				while (col + wordLocation.length < this->numCols && this->matrix[row][col + wordLocation.length] == CROSSWORD_LETTER_UNKNOWN)
					wordLocation.length++;

				if (wordLocation.length > 1)
					wordLocationArray.push_back(wordLocation);
			}

			if ((row == 0 || this->matrix[row - 1][col] == CROSSWORD_ALWAYS_BLANK) && this->matrix[row][col] == CROSSWORD_LETTER_UNKNOWN)
			{
				WordLocation wordLocation;
				wordLocation.location.row = row;
				wordLocation.location.col = col;
				wordLocation.orientation = WordOrientation::DOWN;
				wordLocation.length = 0;

				while (row + wordLocation.length < this->numRows && this->matrix[row + wordLocation.length][col] == CROSSWORD_LETTER_UNKNOWN)
					wordLocation.length++;

				if (wordLocation.length > 1)
					wordLocationArray.push_back(wordLocation);
			}
		}
	}
}

void PuzzleMatrix::GetAllLocationsWithLetter(std::vector<Location>& locationArray, unsigned char letter) const
{
	locationArray.clear();

	for (int row = 0; row < this->numRows; row++)
	{
		for (int col = 0; col < this->numCols; col++)
		{
			if (this->matrix[row][col] == letter)
			{
				Location location;
				location.row = row;
				location.col = col;

				locationArray.push_back(location);
			}
		}
	}
}

bool PuzzleMatrix::HasHole() const
{
	for (int row = 0; row < this->numRows; row++)
	{
		for (int col = 0; col < this->numCols; col++)
		{
			if (this->matrix[row][col] == CROSSWORD_LETTER_UNKNOWN)
			{
				if ((row == 0 || this->matrix[row - 1][col] == CROSSWORD_ALWAYS_BLANK) &&
					(row == this->numRows - 1 || this->matrix[row + 1][col] == CROSSWORD_ALWAYS_BLANK) &&
					(col == 0 || this->matrix[row][col - 1] == CROSSWORD_ALWAYS_BLANK) &&
					(col == this->numCols - 1 || this->matrix[row][col + 1] == CROSSWORD_ALWAYS_BLANK))
				{
					return true;
				}
			}
		}
	}

	return false;
}

std::string PuzzleMatrix::GetWordAt(const WordLocation& wordLocation) const
{
	std::string word;

	for (int i = 0; i < wordLocation.length; i++)
		word += this->GetLetter(wordLocation.GetLocationAt(i));

	return word;
}

Location WordLocation::GetLocationAt(int i) const
{
	assert(0 <= i && i < this->length);

	Location letterLocation = this->location;

	switch (this->orientation)
	{
	case WordOrientation::ACROSS:
		letterLocation.col += i;
		break;
	case WordOrientation::DOWN:
		letterLocation.row += i;
		break;
	default:
		assert(false);
		break;
	}

	return letterLocation;
}