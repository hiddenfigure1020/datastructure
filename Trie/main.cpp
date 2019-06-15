#include <iostream>
#include <string>
#include <stdlib.h>

#define ALPHABET_SIZE 26

class Node
{
private:
	Node ** children;
	bool isEndOfWord;
public:
	Node() :
		children(new Node * [ALPHABET_SIZE]),
		isEndOfWord(false)
	{
		for (int i = 0; i < ALPHABET_SIZE; ++i)
			children[i] = NULL;
	}

	Node(const Node & other) :
		children(other.children),
		isEndOfWord(other.isEndOfWord)
	{

	}

	~Node()
	{

	}
public:
	Node * GetChild(const int & index) const
	{
		return children[index];
	}

	void AddChild(Node * child, const int & index)
	{
		children[index] = child;
	}

	void SetEndOfWord(const bool & isEnd)
	{
		isEndOfWord = isEnd;
	}

	bool IsEndOfWord() const
	{
		return isEndOfWord;
	}

};

class ITrie
{
public:
	virtual  void Insert(const std::string & key) = 0;
	virtual bool Search(const std::string & key) = 0;
};

class IHelper
{
public:
	virtual void Suggest(const std::string & key) = 0;

};

class Trie : public ITrie, IHelper
{
private:
	Node * root;
public:
	Trie() :
		root(new Node()) //root is always empty node
	{

	}

	Trie(const Trie & other) : 
		root(other.root)
	{

	}

	~Trie()
	{

	}

public:

	static int GetAlphabetIndex(const char & c) // index Lowercase
	{
		int index = (int)c;
		if (c < (int)'a')
			index += 32;

		return abs(index - int('a'));
	}

	void Insert(const std::string & key)
	{
		Node * crawl = root;

		for (int i = 0; i < key.size(); ++i)
		{
			int key_index = GetAlphabetIndex(key[i]); //get index in alphabet

			Node * child =  crawl->GetChild(key_index);

			if (NULL == child)
			{
				crawl->AddChild(new Node(), key_index);
			}
			
			crawl = crawl->GetChild(key_index);
		}
		crawl->SetEndOfWord(true);
	}

	bool Search(const std::string & key)
	{
		Node * crawl = root;

		for (int i = 0; i < key.size(); ++i)
		{
			int key_index = GetAlphabetIndex(key[i]); //get index in alphabet

			Node * child = crawl->GetChild(key_index);

			if (NULL == child)
			{
				return false;
			}
			crawl = child;
		}

		return (crawl != NULL && crawl->IsEndOfWord());
	}

	void Suggest(const std::string & key)
	{

	}

};


static void Search(Trie * trie, std::string key)
{
	if (trie->Search(key) == true)
		std::cout << "\nFound " << key;
	else
		std::cout << "\nNot found " << key;
}


int main()
{

	Trie * trie = new Trie();

	trie->Insert("H");
	trie->Insert("He");
	trie->Insert("Hell");
	trie->Insert("Hello");
	trie->Insert("Wo");
	trie->Insert("World");

	Search(trie, "Hello");
	Search(trie, "World");
	Search(trie, "Hi");
	Search(trie, "Wow");

	return 0;
}
