#include <iostream>
#include <conio.h>
#include <string>
#include <stack>
#include <fstream>
#include <list>

//This Trie allow all char >= 32 && <= 94 depend on ASCII
#define CHAR_SIZE				94
#define FIRST_CHAR				' '
#define SOURCE_NAME				"Dictionary.txt"

#define BACK_SPACE_KEY			8
#define ENTER_KEY				13
#define WAITTING_INPUT_TIME		3.0

using namespace std;

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
};

class HelperFunction
{
public:

	static string RemoveLast(const string & value)
	{
		string result = value;
		result = result.erase(result.length() - 1, 1);
		return result;
	}

	static list<string> GetSubString(const string & str, const int & length)
	{
		list<string> result = list<string>();

		for (int i = 0; i + length <= str.length(); ++i)
		{
			string sub_str = str.substr(0, length);
			result.push_back(sub_str);
		}
		return result;
	}
};

class Trie
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

	void Insert(const string & key)
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

	Node * Search(const string & key)
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

	void GetValidWords(Node * root_node, list<string> & result, string str = "")
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

	void Search()
	{
		string value;
		list<string> valid_words = list<string>();

		while (true)
		{
			valid_words.clear();
			cout << "\nPlease type search queries: " << value;
			int x = _getch();

			if (BACK_SPACE_KEY == x)
			{
				value = HelperFunction::RemoveLast(value);
			}
			else
				if (ENTER_KEY == x)
				{
					break;
				}
				else
					value.push_back((char)x);

			Node * node = Search(value);

			if (NULL == node)
			{
				cout << "\n" << value << " not found!";
				cout << "\nDo you mean:";
				Recommendation(value);
				continue;
			}
			else
			{
				cout << "\nYour options are:";
			}
			GetValidWords(node, valid_words, value);
			for (list<string>::const_iterator ci = valid_words.begin(); ci != valid_words.end(); ++ci)
			{
				cout << "\n" << *ci;
			}
		}
	}

	void Recommendation(string val)
	{
		list<string> valid_words = list<string>();
		GetValidWords(root, valid_words);
		list<string> similar_words = list<string>();

		for (int i = val.length(); i > 1; --i)
		{
			list<string> sub_strs = HelperFunction::GetSubString(val, i);
			
			for (list<string>::const_iterator valid_word = valid_words.begin(); 
				valid_word != valid_words.end(); ++valid_word)
			{

				for (list<string>::const_iterator sub_str = sub_strs.begin();
					sub_str != sub_strs.end(); ++sub_str)
				{
					
					if ((*valid_word).find(*sub_str, (*sub_str).length()) != string::npos)
					{
						similar_words.push_back(*valid_word);
						break;
					}
				}
				if (similar_words.size() == 3)
					break;
			}
		}

		for (list<string>::const_iterator word = similar_words.begin();
			word != similar_words.end(); ++word)
		{
			cout << "\n" << *word;
		}
	}

};

class FileReader
{
private:
	string file_path;
public:
	FileReader(const string & path) :
		file_path(path)
	{

	}
public:

	list<string> ReadLine()
	{
		list<string> lines = list<string>();
		ifstream file;
		file.open(file_path.c_str(), ios_base::in);
		if (file.is_open())
		{
			string line;
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
	list<string> lines = file_reader.ReadLine();

	for (list<string>::const_iterator ci = lines.begin(); ci != lines.end(); ++ci)
	{
		trie->Insert(*ci);
	}

	trie->Search();

	return 0;
}

