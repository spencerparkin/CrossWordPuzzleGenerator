#include "CrossWord/WordBank.h"
#include <fstream>

using namespace CrossWord;

WordBank::WordBank()
{
}

/*virtual*/ WordBank::~WordBank()
{
}

bool WordBank::Load(const std::string& filePath)
{
	std::ifstream fileStream;
	fileStream.open(filePath, std::ios::in);
	if (!fileStream.is_open())
		return false;

	this->wordArray.clear();

	std::string word;
	while (std::getline(fileStream, word))
		this->wordArray.push_back(word);

	fileStream.close();
	return true;
}