#include <Windows.h>

#include "RequestWorker.h"
#include "PostOffice.h"

RequestWorker::RequestWorker()
{
	// Do Nothing
}

RequestWorker::RequestWorker(OrderQueue *oq)
{
	msgQueue = MessageQueue::getInstance();
	this->orderQueue = oq;
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
			ptr->orderQueue->Enqueue((OrderData *)(data->data));
			break;
		case CANCEL_ORDER:
			{
				bool isCanceled = ptr->cancelOrder((OrderData *)(data->data));
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
			break;
		case PAY:
			break;
		case LOGIN:
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

bool RequestWorker::cancelOrder(OrderData *data)
{
	return false;
}