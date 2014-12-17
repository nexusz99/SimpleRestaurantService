#pragma once

#include "MessageQueue.h"
#include "DataSet\OrderQueue.h"

#define WAIT_TIME 300

class RequestWorker
{
public:	
	MessageQueue* msgQueue;
	RequestWorker();	
	void start();
	void sendMessageToClient();
	void order(MessageQueueData* data);
	void cancelOrder(MessageQueueData* data);
	void pay(MessageQueueData* data);
	void login(MessageQueueData* data);
	void signup(MessageQueueData* data);

private:
};

