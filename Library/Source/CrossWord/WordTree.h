#pragma once

#include <string>
#include <vector>

namespace CrossWord
{
	/**
	 * 
	 */
	class WordTree
	{
	public:
		WordTree();
		virtual ~WordTree();

		void Clear();
		void AddWord(const std::string& word);
		bool IsWord(const std::string& word) const;

	private:

		class Node
		{
		public:
			Node();
			virtual ~Node();

			void AddWord(const unsigned char* wordBuffer, int wordLength, int i);

			Node* nodeArray[256];
			bool atWordEnd;
		};

		Node* rootNode;
	};
}