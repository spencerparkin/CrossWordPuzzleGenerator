#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "CrossWord/WordTree.h"

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

		void Clear();
		bool Load(const std::string& filePath);
		bool IsWord(const std::string& word) const;
		const std::vector<std::string>* GetAllWordsOfLength(int length) const;

	private:
		struct Bucket
		{
			int wordLength;
			std::vector<std::string> wordArray;
		};

		WordTree wordTree;

		std::unordered_map<int, std::shared_ptr<Bucket>> bucketMap;
	};
}