#pragma once

#include <typeinfo>

template<class ItemType>
struct LLNode
{
    ItemType info;
    LLNode* next;
};

template <class ItemType>
class LinkedList
{
public:
	LinkedList();
	~LinkedList();

	void InsertItem(ItemType item);

	LinkedList(LinkedList<ItemType> &src);
	void operator=(const LinkedList<ItemType> &src);

	int getCount();

	ItemType RetriveItem(ItemType i);

	void ResetIterator();

	ItemType iter();

private:
	LLNode<ItemType> *listData;
	LLNode<ItemType> *currentPos;
	int count;
};




template <class ItemType>
LinkedList<ItemType>::LinkedList()
{
	listData = NULL;
	count = 0;
}

template <class ItemType>
LinkedList<ItemType>::LinkedList(LinkedList<ItemType> &src)
{
	this = src;
}

template <class ItemType>
void LinkedList<ItemType>::operator=(const LinkedList<ItemType> &src)
{

	count = src.count;
	listData = NULL;
	if(count==0)
		return;
	listData = new LLNode<ItemType>;
	LLNode<ItemType> *srcHead = src.listData;
	listData->info = srcHead->info;
	listData->next = NULL;

	srcHead = srcHead->next;
	
	for(int i = 1; i < count; i++)
	{
		LLNode<ItemType> *tmp = new LLNode<ItemType>;
		tmp->info = srcHead->info;
		tmp->next = listData;
		listData = tmp;
		srcHead = srcHead->next;
	}
}

template <class ItemType>
LinkedList<ItemType>::~LinkedList()
{
	LLNode<ItemType>* tempPtr;
	while(listData != NULL)
	{
		tempPtr = listData;
		listData = listData->next;
		delete tempPtr;
	}
}

template <class ItemType>
void LinkedList<ItemType>::InsertItem(ItemType item)
{
	LLNode<ItemType>* location;
	location = new LLNode<ItemType>;
	location->info = item;
	location->next = listData;
	listData = location;
	count++;
}

template <class ItemType>
ItemType LinkedList<ItemType>::RetriveItem(ItemType i)
{
	ResetIterator();
	ItemType item;
	while((item = iter()) != NULL)
	{
		if(typeid(char*) == typeid(ItemType))
		{
			if(strcmp(item, i) == 0)
				return item;
		}
		else if(item == i)
		{
			return item;
		}
	}
}

template <class ItemType>
void LinkedList<ItemType>::ResetIterator()
{
	currentPos = listData;
}

template <class ItemType>
ItemType LinkedList<ItemType>::iter()
{
	if(currentPos == NULL)
	{
		if(typeid(char*) == typeid(ItemType))
			return NULL;
		else
			throw 1;
	}
	ItemType item = currentPos->info;
	currentPos = currentPos->next;
	return item;
}

template <class ItemType>
int LinkedList<ItemType>::getCount()
{
	return count;
}
