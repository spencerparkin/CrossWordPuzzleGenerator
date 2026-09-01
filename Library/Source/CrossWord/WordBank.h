#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
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

		struct WordProfile
		{
			void AddCharacteristic(int i, unsigned char letter);
			void Sort();	// This only needs to be called if the characteristics were added in an unsorted order.
			std::string GetKey() const;		// This assumes the tuples are already sorted!
			bool WordMatchesProfile(const std::string& word) const;

			std::vector<std::tuple<int, unsigned char>> tupleArray;
		};

		void Clear();
		bool Load(const std::string& filePath);
		bool IsWord(const std::string& word) const;
		const std::vector<std::string>* GetAllWordsOfLength(int length) const;
		const std::vector<std::string>* GetAllWordsOfLengthWithProfile(int length, const WordProfile& profile) const;

	private:
		
		struct Collection
		{
			std::vector<std::string> wordArray;
		};

		struct Bucket
		{
			std::vector<std::string> wordArray;
			std::unordered_map<std::string, std::shared_ptr<Collection>> collectionMap;
		};

		WordTree wordTree;

		std::unordered_map<int, std::shared_ptr<Bucket>> bucketMap;
	};
}