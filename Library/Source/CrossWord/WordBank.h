#pragma once

#include <string>
#include <vector>

namespace CrossWord
{
	/**
	 * An instance of this class stores a database of words and
	 * accelerates certain tasks related to such a database, such
	 * as determining if a given sequence of letters is a word.
	 */
	class WordBank
	{
	public:
		WordBank();
		virtual ~WordBank();

		bool Load(const std::string& filePath);

	private:
		std::vector<std::string> wordArray;
	};
}