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

	private:
		struct Bucket
		{
			WordTree wordTree;
			int wordLength;
		};

		WordTree wordTree;

		std::unordered_map<int, std::shared_ptr<Bucket>> bucketMap;
	};
}