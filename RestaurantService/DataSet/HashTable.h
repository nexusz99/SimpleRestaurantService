#pragma once

#include <string>
#include "../md5.h"
#include "LinkedList.h"

using namespace std;

template <class ItemType2>
class HashTable
{
private:
	ItemType2 *table;
	int count;

public:
	HashTable();

	ItemType2 get(string key);

	void put(string key, ItemType2 value);

	LinkedList<char*>* getKeys();

	int getCount();

	~HashTable();

private:
	LinkedList<char*>* keyList;
};


template<class ItemType2>
HashTable<ItemType2>::HashTable()
{
	table = new ItemType2[1000];
	count = 0;
	keyList = new LinkedList<char*>;
}

template<class ItemType2>
ItemType2 HashTable<ItemType2>::get(string key)
{
	BigInteger hash = md5ToBigInt(md5(key));
	int index = stol((hash % 1000).getNumber());
	return table[index];
}

template<class ItemType2>
void HashTable<ItemType2>::put(string key, ItemType2 value)
{
	char *buf = new char[key.length()+1];
	strcpy(buf, key.c_str());
	if(keyList->RetriveItem(buf)!=NULL)
		return;
	BigInteger hash = md5ToBigInt(md5(key));
	int index = stol((hash % 1000).getNumber());
	table[index] = value;
	count++;
	
	keyList->InsertItem(buf);
}

template<class ItemType2>
LinkedList<char *>* HashTable<ItemType2>::getKeys()
{
	return keyList;
}

template<class ItemType2>
int HashTable<ItemType2>::getCount()
{
	return count;
}

template<class ItemType2>
HashTable<ItemType2>::~HashTable()
{
	delete [] table;
}