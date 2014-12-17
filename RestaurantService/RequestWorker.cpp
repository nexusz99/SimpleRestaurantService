#include <Windows.h>

#include "RequestWorker.h"
#include "PostOffice.h"
#include "RestraurantManager.h"
#include "DataSet\PayData.h"

extern RestraurantManager rm;
extern OrderQueue* oq;

RequestWorker::RequestWorker()
{
	msgQueue = MessageQueue::getInstance();
}

DWORD WINAPI work(LPVOID lpParameter)
{
	RequestWorker *ptr = (RequestWorker*)lpParameter;	
	while(true)
	{
		MessageQueueData* data = ptr->msgQueue->Dequeue();
		if(data == NULL)
		{
			Sleep(WAIT_TIME);
			continue;
		}

		switch(data->msgType)
		{
		case ORDER:
			ptr->order(data);
			break;
		case CANCEL_ORDER:
			ptr->cancelOrder(data);
			break;
		case PAY:
			ptr->pay(data);
			break;
		case LOGIN:
			ptr->login(data);
			break;
		case SIGNUP:
			ptr->signup(data);
			break;
		}

		Sleep(WAIT_TIME);
	}
	return 0;
}

void RequestWorker::start()
{
	CreateThread(NULL, 0, work, this, 0, 0);	
}

void RequestWorker::cancelOrder(MessageQueueData* data)
{	
	bool isCanceled = oq->DeleteItem(data);
	//Generate Message
	string msg = "";
	Json json;
	CancelOrderMsg comsg;
	comsg.result = isCanceled;
	comsg.msg = isCanceled ? "주문이 취소되었습니다." : "이미 조리중입니다";
	msg = json.serialize((Serializable&)comsg);

	// Deligate send message to PostOffice
	PostOffice officer;
	Letter l;
	l.client_id = data->client_id;
	l.msg = msg;
	officer.sendMessage(l);
}

void RequestWorker::order(MessageQueueData* data)
{
	oq->Enqueue(data);
}

void RequestWorker::pay(MessageQueueData* data)
{
	PayRequest *p = (PayRequest*)data->data;
	PayData *pd = new PayData();
	pd->username = data->client_id;
	pd->amount = p->amount;
	pd->cash = p->cost;
	pd->item = rm.getItemNameById(p->itemno);

	rm.writePaymentInfo(pd);
}

void RequestWorker::login(MessageQueueData* data)
{
	User* u = (User*)(data->data);
	bool valid = rm.verifyLogin(u->username, u->password);

	PostOffice officer;
	Letter l;
	l.client_id = data->client_id;
	l.msg = valid ? "1" : "0";
	officer.sendMessage(l);
}

void RequestWorker::signup(MessageQueueData* data)
{
	User* u = (User*)(data->data);

	PostOffice officer;
	Letter l;
	l.client_id = u->username;
	if(rm.createNewUser(u->username, u->password))
	{
		l.msg = "1";
		
	}
	else
	{
		l.msg = "0";
	}
	officer.sendMessage(l);
}