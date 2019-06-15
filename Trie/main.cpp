#include <iostream>
#include <string>
#include <stdlib.h>
#include <stack>

#define ALPHABET_SIZE			26
#define SOURCE_NAME				"Dictionary.txt "

class Node
{
private:
	Node ** children;
	bool isEndOfWord;
public:
	Node() :
		children(new Node *[ALPHABET_SIZE]),
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
		for (int i = 0; i < ALPHABET_SIZE; ++i)
			if (NULL != children[i])
			{
				delete children[i];
			}
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
	
	int GetNumOfChildren() const
	{
		int num = 0;
		for (int i = 0; i < ALPHABET_SIZE; ++i)
			if (NULL != children[i])
				num++;
		return num;
	}

	void RemoveChild(Node * child)
	{
		for (int i = 0; i < ALPHABET_SIZE; ++i)
			if (child == children[i])
			{
				children[i] = NULL;
				return;
			}
	}

};

class ITrie
{
public:
	virtual  void Insert(const std::string & key) = 0;
	virtual bool Search(const std::string & key) = 0;
	virtual void Delete(const std::string & key) = 0;
	virtual void Display(Node * root_node, std::string result) = 0;
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
		delete root;
	}

public:

	Node * GetRoot() const
	{
		return root;
	}

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

			Node * child = crawl->GetChild(key_index);

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

	void Delete(const std::string & key)
	{
		Node * crawl = root;

		std::stack<Node*> nodes = std::stack<Node*>();
		
		for (int i = 0; i < key.size(); ++i)
		{
			int key_index = GetAlphabetIndex(key[i]); //get index in alphabet

			Node * child = crawl->GetChild(key_index);

			if (NULL == child)
			{
				return; //can not found this word to delete
			}
			nodes.push(child);
			crawl = child;
		}

		if (true == crawl->IsEndOfWord() && NULL != crawl)
		{
			Node * mark = NULL;
			Node * pre_mark = NULL;
			while (!nodes.empty())
			{
				Node * temp = nodes.top();

				if (((temp->IsEndOfWord() == true && temp->GetNumOfChildren() == 0) ||
					(temp->IsEndOfWord() == false && temp->GetNumOfChildren() == 1)) && temp != root)
				{
					mark = temp;
				}
				else
				{
					pre_mark = temp;
					break;
				}
				nodes.pop();
			}

			pre_mark->RemoveChild(mark);
			delete mark;
		}
	}

	void Display(Node * root_node, std::string result = "")
	{
		if (true == root_node->IsEndOfWord())
		{
			std::cout << "\n" << result;
		}

		for (int i = 0; i < ALPHABET_SIZE; ++i)
		{
			Node * child = root_node->GetChild(i);

			if (NULL != child)
			{
				result.push_back(i + 'a');
				Display(child, result);
				result = result.erase(result.length() - 1, 1);
			}
		}
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

	std::cout << "\nBefore";
	trie->Display(trie->GetRoot());

	trie->Delete("Hello");

	std::cout << "\n\nAfter delete";
	trie->Display(trie->GetRoot());

	return 0;
}

