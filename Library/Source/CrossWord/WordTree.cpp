#include "WordTree.h"
#include <assert.h>

using namespace CrossWord;

//------------------------------------ WordTree ------------------------------------

WordTree::WordTree()
{
	this->rootNode = nullptr;
}

/*virtual*/ WordTree::~WordTree()
{
	this->Clear();
}

void WordTree::Clear()
{
	delete this->rootNode;
	this->rootNode = nullptr;
}

void WordTree::AddWord(const std::string& word)
{
	if (!this->rootNode)
		this->rootNode = new Node();

	this->rootNode->AddWord((unsigned char*)word.c_str(), (int)word.length(), 0);
}

bool WordTree::IsWord(const std::string& word) const
{
	if (!this->rootNode)
		return false;

	const unsigned char* wordBuffer = (unsigned char*)word.c_str();
	int wordLength = (int)word.length();
	
	int i = 0;
	const Node* node = this->rootNode;
	while (node)
	{
		if (i >= wordLength)
			break;

		unsigned char letter = wordBuffer[i++];
		node = node->nodeArray[letter];
	}

	return node && node->atWordEnd;
}

//------------------------------------ WordTree::Node ------------------------------------

WordTree::Node::Node()
{
	for (int i = 0; i < 256; i++)
		this->nodeArray[i] = nullptr;

	this->atWordEnd = false;
}

/*virtual*/ WordTree::Node::~Node()
{
	for (int i = 0; i < 256; i++)
		delete this->nodeArray[i];
}

void WordTree::Node::AddWord(const unsigned char* wordBuffer, int wordLength, int i)
{
	if (i >= wordLength)
	{
		this->atWordEnd = true;
		return;
	}

	unsigned char letter = wordBuffer[i];

	if (!this->nodeArray[letter])
		this->nodeArray[letter] = new Node();

	this->nodeArray[letter]->AddWord(wordBuffer, wordLength, i + 1);
}