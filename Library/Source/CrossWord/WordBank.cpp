#include "CrossWord/WordBank.h"
#include <fstream>
#include <algorithm>
#include <format>

using namespace CrossWord;

//-------------------------------------- WordBank --------------------------------------

WordBank::WordBank()
{
	this->numWords = 0;
}

/*virtual*/ WordBank::~WordBank()
{
}

void WordBank::Clear()
{
	this->wordTree.Clear();
	this->bucketMap.clear();
	this->numWords = 0;
}

int WordBank::GetNumWords() const
{
	return this->numWords;
}

bool WordBank::Load(const std::string& filePath, std::function<bool(double)> progressCallback)
{
	std::ifstream fileStream;
	fileStream.open(filePath, std::ios::in | std::ios::ate);
	if (!fileStream.is_open())
		return false;

	std::streamsize numTotalBytes = fileStream.tellg();
	fileStream.seekg(0, std::ios::beg);

	std::string word;
	while (std::getline(fileStream, word))
	{
		this->numWords++;
		this->wordTree.AddWord(word);

		int wordLength = (int)word.length();
		std::shared_ptr<Bucket> bucket;
		auto pair = this->bucketMap.find(wordLength);
		if (pair != this->bucketMap.end())
			bucket = pair->second;
		else
		{
			bucket = std::make_shared<Bucket>();
			this->bucketMap.insert(std::pair(wordLength, bucket));
		}

		bucket->wordArray.push_back(word);
		
		if (progressCallback)
		{
			std::streamsize numBytesRead = fileStream.tellg();
			double progress = double(numBytesRead) / double(numTotalBytes);
			if (progressCallback(progress))
				return false;
		}
	}

	fileStream.close();
	return true;
}

bool WordBank::IsWord(const std::string& word) const
{
	return this->wordTree.IsWord(word);
}

const std::vector<std::string>* WordBank::GetAllWordsOfLength(int length) const
{
	auto pair = this->bucketMap.find(length);
	if (pair == this->bucketMap.end())
		return nullptr;

	return &pair->second->wordArray;
}

const std::vector<std::string>* WordBank::GetAllWordsOfLengthWithProfile(int length, const WordProfile& profile) const
{
	auto pairA = this->bucketMap.find(length);
	if (pairA == this->bucketMap.end())
		return nullptr;

	const std::vector<std::string>* wordArray = nullptr;

	if (profile.tupleArray.size() == 0)
		wordArray = &pairA->second->wordArray;
	else
	{
		std::string key = profile.GetKey();

		auto pairB = pairA->second->collectionMap.find(key);
		if (pairB != pairA->second->collectionMap.end())
			wordArray = &pairB->second->wordArray;
		else
		{
			auto collection = std::make_shared<Collection>();

			for (const std::string& word : pairA->second->wordArray)
				if (profile.WordMatchesProfile(word))
					collection->wordArray.push_back(word);

			wordArray = &collection->wordArray;
			pairA->second->collectionMap.insert(std::pair(key, collection));
		}
	}

	return wordArray;
}

//-------------------------------------- WordBank::WordProfile --------------------------------------

void WordBank::WordProfile::Clear()
{
	this->tupleArray.clear();
}

void WordBank::WordProfile::AddCharacteristic(int i, unsigned char letter)
{
	this->tupleArray.push_back(std::tuple<int, unsigned char>(i, letter));
}

void WordBank::WordProfile::Sort()
{
	std::sort(this->tupleArray.begin(), this->tupleArray.end(), [](const std::tuple<int, unsigned char>& tupleA, const std::tuple<int, unsigned char>& tupleB) -> bool
		{
			return std::get<0>(tupleA) < std::get<0>(tupleB);
		});
}

std::string WordBank::WordProfile::GetKey() const
{
	std::string key;

	for (const std::tuple<int, unsigned char>& tuple : this->tupleArray)
		key += std::format("{}{}", std::get<0>(tuple), (char)std::get<1>(tuple));

	return key;
}

bool WordBank::WordProfile::WordMatchesProfile(const std::string& word) const
{
	int length = (int)word.length();

	for (const std::tuple<int, unsigned char>& tuple : this->tupleArray)
	{
		int i = std::get<0>(tuple);
		unsigned char letter = std::get<1>(tuple);
		
		if (i < 0 || i >= length)
			return false;

		if (word[i] != letter)
			return false;
	}

	return true;
}