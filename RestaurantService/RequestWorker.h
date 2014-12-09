#pragma once

#include "MessageQueue.h"
#include "DataSet\OrderQueue.h"

#define WAIT_TIME 500

class RequestWorker
{
public:	
	MessageQueue* msgQueue;
	OrderQueue *orderQueue;

	RequestWorker(OrderQueue *oq);
	void start();
	bool cancelOrder(OrderData *data);
	void sendMessageToClient();

private:
	RequestWorker();
};

