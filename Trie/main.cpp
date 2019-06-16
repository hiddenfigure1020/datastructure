#include <iostream>
#include<conio.h>
#include <string>
#include <stdlib.h>
#include <stack>
#include <fstream>
#include <list>

//This Trie allow all char >= 32 && <= 94 depend on ASCII
#define CHAR_SIZE				94
#define FIRST_CHAR				' '
#define SOURCE_NAME				"Dictionary.txt"

#define BACK_SPACE_KEY			8

class Node
{
private:
	Node ** children;
	bool isEndOfWord;
public:
	Node() :
		children(new Node *[CHAR_SIZE]),
		isEndOfWord(false)
	{
		for (int i = 0; i < CHAR_SIZE; ++i)
			children[i] = NULL;
	}

	Node(const Node & other) :
		children(other.children),
		isEndOfWord(other.isEndOfWord)
	{

	}

	~Node()
	{
		for (int i = 0; i < CHAR_SIZE; ++i)
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
		for (int i = 0; i < CHAR_SIZE; ++i)
			if (NULL != children[i])
				num++;
		return num;
	}

	void RemoveChild(Node * child)
	{
		for (int i = 0; i < CHAR_SIZE; ++i)
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
	virtual Node* Search(const std::string & key) = 0;
	virtual void Delete(const std::string & key) = 0;
	virtual void GetValidWords(Node * root_node, std::list<std::string> & result, std::string str) = 0;
};

class IHelper
{
public:
	virtual void Suggest() = 0;

};

class HelperFunction
{
public:

	static std::string RemoveLast(const std::string & value)
	{
		std::string result = value;
		result = result.erase(result.length() - 1, 1);
		return result;
	}
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

	static int GetAlphabetIndex(const char & c)
	{
		int index = (int)c;
		return index - (int)FIRST_CHAR;
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

	Node * Search(const std::string & key)
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

		return crawl;
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

	void GetValidWords(Node * root_node, std::list<std::string> & result, std::string str = "")
	{
		if (true == root_node->IsEndOfWord())
		{
			result.push_back(str);
		}

		for (int i = 0; i < CHAR_SIZE; ++i)
		{
			Node * child = root_node->GetChild(i);

			if (NULL != child)
			{
				str.push_back(i + FIRST_CHAR);
				GetValidWords(child, result, str);
				str = HelperFunction::RemoveLast(str);
			}
		}
	}


	void Suggest()
	{
		std::string value;

		while (true)
		{
			std::cout << "\nPlease type search queries:";
			int x = _getch();

			if (BACK_SPACE_KEY == x)
			{
				value = HelperFunction::RemoveLast(value);
			}
			else
				value.push_back((char)x);
			std::cout << " " << value;

			Node * node = Search(value);

			if (NULL == node)
			{
				std::cout << "\nDo you mean:";

				std::string temp_str = value;
				while (NULL == node)
				{
					temp_str = HelperFunction::RemoveLast(temp_str);

					node = Search(temp_str);

					if (temp_str.empty())
					{
						std::cout << "\nCan not detec!";
						continue;
					}
				}
			}
			else
			{
				std::cout << "\nYour options are:";
			}
			std::list<std::string> valid_words = std::list<std::string>();
			GetValidWords(node, valid_words, value);
			for (std::list<std::string>::const_iterator ci = valid_words.begin(); ci != valid_words.end(); ++ci)
			{
				std::cout << "\n" << *ci;
			}
		}
	}

};


static void Search(Trie * trie, std::string key)
{
	Node * node = trie->Search(key);
	if (node != NULL && node->IsEndOfWord())
		std::cout << "\nFound " << key;
	else
		std::cout << "\nNot found " << key;
}

class FileReader
{
private:
	std::string file_path;
public:
	FileReader(const std::string & path) :
		file_path(path)
	{

	}
public:

	std::list<std::string> ReadLine()
	{
		std::list<std::string> lines = std::list<std::string>();
		std::ifstream file;
		file.open(file_path.c_str(), std::ios_base::in);
		if (file.is_open())
		{
			std::string line;
			while (getline(file, line))
			{
				lines.push_back(line);
			}
			file.close();
		}
		return lines;
	}

};


int main()
{

	Trie * trie = new Trie();

	FileReader file_reader = FileReader(SOURCE_NAME);
	std::list<std::string> lines = file_reader.ReadLine();

	for (std::list<std::string>::const_iterator ci = lines.begin(); ci != lines.end(); ++ci)
	{
		trie->Insert(*ci);
	}

	//trie->Display(trie->GetRoot());

	trie->Suggest();

	return 0;
}

