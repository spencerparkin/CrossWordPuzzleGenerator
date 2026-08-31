#pragma once

#include <string>
#include <vector>

#define CROSSWORD_ALWAYS_BLANK		219
#define CROSSWORD_LETTER_UNKNOWN	250

namespace CrossWord
{
	enum WordOrientation
	{
		ACROSS,
		DOWN
	};

	struct Location
	{
		int row, col;
	};

	struct WordLocation
	{
		Location location;
		int length;
		WordOrientation orientation;

		Location GetLocationAt(int i) const;
	};

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
		bool IsValidLocation(int row, int col) const;
		bool IsValidLocation(const Location& location) const;
		Location GetSymmetricLocation(const Location& location) const;
		unsigned char GetLetter(int row, int col) const;
		unsigned char GetLetter(const Location& location) const;
		void SetLetter(int row, int col, unsigned char letter);
		void SetLetter(const Location& location, unsigned char letter);
		bool HasHole() const;
		std::string GetWordAt(const WordLocation& wordLocation) const;

		void GetAllWordLocations(std::vector<WordLocation>& wordLocationArray) const;
		void GetAllLocationsWithLetter(std::vector<Location>& locationArray, unsigned char letter) const;

		std::string Print() const;

	private:
		unsigned char** matrix;
		int numRows;
		int numCols;
	};
}