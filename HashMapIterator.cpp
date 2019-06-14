#include <iostream>

const int TABLE_SIZE = 7;

class Iterator;
class HashMapIterator;
class HashMap;
class HashEntry;
class ManualHashMap;

class HashEntry
{
private:
	int key;
	int value;

public:
	HashEntry(int _key, int _value) :
		key(_key),
		value(_value)
	{
	}

	~HashEntry()
	{
	}

public:

	void print() const
	{
		std::cout << "\nKey: " << key << " value: " << value;
	}

	int getKey() const
	{
		return key;
	}

	int getValue() const
	{
		return value;
	}

};

class Iterator
{
public:
	virtual void  first() = 0;
	virtual void  next() = 0;
	virtual bool isDone() = 0;
	virtual HashEntry * currentItem() = 0;
};


class Iterable
{
public:
	virtual Iterator * getIterator() = 0;
};

class HashMap : public Iterable
{
private:
	HashEntry **table;
	int current_size;
	int size;

public:
	HashMap(const int & _size) :
		size(_size),
		current_size(0)
	{
		table = new HashEntry*[size];
		for (int i = 0; i < size; i++)
			table[i] = NULL;
	}

	~HashMap()
	{
		for (int i = 0; i < size; i++)
			if (table[i] != NULL)
				delete table[i];
		delete[] table;
	}

public:

	friend class HashMapIterator;

	Iterator* getIterator();

	bool isFull()
	{
		return current_size * 2 >= size;
	}

	void insert(int keyvalue)
	{
		if (isFull())
		{
			rehash();
		}

		int hash = (keyvalue % size);
		while (table[hash] != NULL && table[hash]->getKey() != keyvalue)
			hash = (hash + 1) % size;

		if (table[hash] == NULL)
		{
			table[hash] = new HashEntry(keyvalue, keyvalue);
			std::cout << "\nInsert value: " << keyvalue;
		}
		else
			return;

		current_size++;
		std::cout << "\nCurrent size: " << current_size;
		std::cout << "\nTotal size: " << size;
	}

	int get(int key)
	{
		int hash = (key % size);
		while (table[hash] != NULL && table[hash]->getKey() != key)
			hash = (hash + 1) % size;

		if (table[hash] == NULL)
			return 0;
		else
			return table[hash]->getValue();
	}


	void find(int value)
	{


		int val = get(value);

		if (val == value)
			std::cout << "\nFound value: " << value;
		else
			std::cout << "\ncant find" << value;
	}



private:

	void init(const int & _size)
	{
		size = _size;
		current_size = 0;

		table = new HashEntry*[size];
		for (int i = 0; i < size; i++)
			table[i] = NULL;
	}

	void rehash()
	{
		std::cout << "\nRehash";

		HashEntry ** temp = table;
		int temp_size = size;
		init(nextPrime(size * 2));
		for (int i = 0; i < temp_size; ++i)
		{
			if (temp[i] != NULL)
			{
				insert(temp[i]->getValue());
			}
		}
		delete temp;
	}

	bool isPrime(int n)
	{
		if (n == 2 || n == 3)
			return true;

		if (n == 1 || n % 2 == 0)
			return false;

		for (int i = 3; i * i <= n; i += 2)
			if (n % i == 0)
				return false;

		return true;
	}

	int nextPrime(int n)
	{
		if (n <= 0)
			n = 3;

		if (n % 2 == 0)
			n++;

		for (; !isPrime(n); n += 2)
			;

		return n;
	}
};

class HashMapIterator : public Iterator
{
private:
	 HashMap * hash_map;
	int index;
public:
	HashMapIterator(HashMap * _hash_map) :
		hash_map(_hash_map),
		index(0)
	{

	}
public:
	friend class HashMap;

	void first()
	{
		for (int i = 0; i < hash_map->size; ++i)
		{
			if (NULL != hash_map->table[i])
			{
				index = i;
				return;
			}
		}
	}
	void next()
	{
		for (int i = index + 1; i < hash_map->size; ++i)
		{
			if (NULL != hash_map->table[i])
			{
				index = i;
				return;
			}
		}
	}

	bool isDone()
	{
		return index > hash_map->size;
	}

	HashEntry * currentItem()
	{
		return hash_map->table[index];
	}

};


Iterator* HashMap::getIterator()
{
	return new HashMapIterator(this);
}

class ManualHashMap
{
private:
	Iterator * interator;
public:
	ManualHashMap(Iterator *  _interator) :
		interator(_interator)
	{

	}

public:
	void display()
	{
		interator->first();
		if (!interator->isDone())
		{
			HashEntry * entry = interator->currentItem();
			entry->print();
			interator->next();
		}
	}
};


int main()
{
	HashMap * hash_map = new HashMap(TABLE_SIZE);


	hash_map->insert(121);
	hash_map->insert(81);
	hash_map->insert(16);
	hash_map->insert(100);
	hash_map->insert(25);
	hash_map->insert(0);
	hash_map->insert(1);
	hash_map->insert(9);
	hash_map->insert(4);
	hash_map->insert(36);
	hash_map->insert(64);
	hash_map->insert(49);
	hash_map->find(134);

	ManualHashMap * manual_hashmap = new ManualHashMap(hash_map->getIterator());
	manual_hashmap->display();

	//rehash
	delete hash_map;
	return 0;
}
