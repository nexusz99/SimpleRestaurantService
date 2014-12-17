#include "OrderQueue.h"

#include "../RestraurantManager.h"
#include "../ConsoleLogger.h"
#include <iostream>

using namespace std;

extern RestraurantManager rm;
extern CConsoleLoggerEx orderConsole;

OrderQueue::OrderQueue()
{
	front = NULL;
	rear = NULL;
}

OrderQueue::~OrderQueue()
{
	NodeType *tmp;
	while(front!=NULL)
	{
		tmp = front;
		front = front->next;
		delete tmp;
	}
	rear = NULL;
}

bool OrderQueue::isFull()
{
	NodeType* tmp;
	try
	{
		tmp = new NodeType;
		delete tmp;
		return false;
	}
	catch(bad_alloc exception)
	{
		return true;
	}
}

bool OrderQueue::isEmpty()
{
	return (front == NULL);
}

void OrderQueue::Enqueue(MessageQueueData *data)
{
	if(isFull())
		throw 1;
	else
	{
		NodeType *newNode;
		newNode = new NodeType;
		newNode->info = data;
		newNode->next = NULL;
		if(rear==NULL) 
			front = newNode;
		else
			rear->next = newNode;
		rear = newNode;
	}
}

MessageQueueData* OrderQueue::Dequeue()
{
	MessageQueueData* item;
	if(isEmpty())
		throw 1;
	else
	{
		NodeType *tmp;
		tmp = front;
		item = front->info;
		front = front->next;
		if(front == NULL)
		{
			rear = NULL;
		}
		delete tmp;
		return item;
	}
}


bool OrderQueue::DeleteItem(MessageQueueData* data)
{
	bool flg = false;
	NodeType *storeFront = front;
	NodeType *back=front, *current=front;
	
	OrderData* pData = (OrderData*)data->data;
	while(current!=NULL)
	{
		OrderData* qData = (OrderData*)current->info->data;

		if((current->info->client_id == data->client_id )
			&& (pData->itemNo == qData->itemNo))
		{
			flg = true;
			if(current == front)
			{
				NodeType *tmp = front;
				front = front->next;
				if(front == NULL)
					rear = NULL;
				delete tmp;
				current = front;
				storeFront = front;
				continue;
			}
			else
			{
				NodeType *tmp = current;
				back->next = current->next;
				delete tmp;
				current = back;
			}
		}
		back = current;
		current = current->next;
	}
	front = storeFront;
	return flg;
}

void OrderQueue::print()
{
	NodeType* tmp = front;
	while(tmp!=NULL)
	{
		OrderData* d = (OrderData*)tmp->info->data;
		orderConsole.printf("%s          ", tmp->info->client_id.c_str());
		orderConsole.printf("%s          ", rm.getItemNameById(d->itemNo).c_str());
		orderConsole.printf("%d  WAIT\n", d->amount);
		tmp = tmp->next;
	}
}