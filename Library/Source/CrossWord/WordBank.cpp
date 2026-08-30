#include "CrossWord/WordBank.h"
#include <fstream>

using namespace CrossWord;

WordBank::WordBank()
{
}

/*virtual*/ WordBank::~WordBank()
{
}

void WordBank::Clear()
{
	this->wordTree.Clear();
	this->bucketMap.clear();
}

bool WordBank::Load(const std::string& filePath)
{
	std::ifstream fileStream;
	fileStream.open(filePath, std::ios::in);
	if (!fileStream.is_open())
		return false;

	std::string word;
	while (std::getline(fileStream, word))
	{
		this->wordTree.AddWord(word);

		int wordLength = (int)word.length();
		std::shared_ptr<Bucket> bucket;
		auto pair = this->bucketMap.find(wordLength);
		if (pair != this->bucketMap.end())
			bucket = pair->second;
		else
		{
			bucket = std::make_shared<Bucket>();
			bucket->wordLength = wordLength;
			this->bucketMap.insert(std::pair(wordLength, bucket));
		}

		bucket->wordTree.AddWord(word);
	}

	fileStream.close();
	return true;
}

bool WordBank::IsWord(const std::string& word) const
{
	return this->wordTree.IsWord(word);
}