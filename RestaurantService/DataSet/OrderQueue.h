#pragma once

#include "MessageQueueData.h"

struct NodeType;

class OrderQueue
{
public:
	OrderQueue();
	~OrderQueue();
	void Enqueue(MessageQueueData *data);
	MessageQueueData* Dequeue();
	bool DeleteItem(MessageQueueData *data);

	void print();
private:
	NodeType* front;
	NodeType* rear;
	void MakeEmpty();
	bool isFull();
	bool isEmpty();
};

struct NodeType
{
	MessageQueueData *info;
	NodeType* next;
};