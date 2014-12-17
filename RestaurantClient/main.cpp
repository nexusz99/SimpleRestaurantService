#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#include <random>

#include "../RestaurantService/Config.h"
#include "../RestaurantService/DataSet/MessageQueueData.h"
#include "../RestaurantService/Json.h"
#include "MessageQueue.h"

using namespace std;



void signup(string username)
{
	MessageQueue* queue = MessageQueue::getInstance();
	MessageQueueData data;
	data.client_id = username;
	data.msgType = SIGNUP;

	User *user = new User();
	user->username = username;
	user->password = "1234";
	data.data = (Data*)user;
	queue->Enqueue(data);
}

void order(string username)
{
	MessageQueue* queue = MessageQueue::getInstance();
	MessageQueueData data;
	data.client_id = username;
	data.msgType = ORDER;
	random_device rd;
	srand(rd());
	int random = rand() % 4 +1;

	OrderData *d = new OrderData();
	d->amount = random+2;
	d->itemNo = random;

	data.data = (Data*)d;

	queue->Enqueue(data);
}

void pay(string username)
{
	MessageQueue* queue = MessageQueue::getInstance();
	MessageQueueData data;
	data.client_id = username;
	data.msgType = PAY;
	random_device rd;
	srand(rd());
	int random = rand() % 4 +1;

	PayRequest *p = new PayRequest();
	p->amount = random+2;
	p->cost = rand() % 10000 + 1;
	p->itemno = random;

	data.data = (Data*)p;

	queue->Enqueue(data);
}


DWORD WINAPI simulation(LPVOID lpParameter)
{
	int timestamp = (int)lpParameter;
	random_device rd;
	srand (rd());
	int random = rand() % 99999999 + 1;

	int v = timestamp+random;

	string username = "test-"+to_string((_Longlong)v);

	signup(username);

	Sleep(300);

	for(int i = 0; i < 3; i++)
	{
		order(username);
		Sleep(500);
		pay(username);
		Sleep(500);
	}

	return 0;
}

int main()
{
	const int threadCnt = 10;
	HANDLE  hThreadArray[threadCnt];
	DWORD id[threadCnt];
	time_t ltime;
	time(&ltime);

	int time2 = time(&ltime);

	for(int i = 0; i < threadCnt; i++)
	{
		hThreadArray[i] = CreateThread(NULL, 0, simulation, LPVOID(time2), 0, &id[i]);
		Sleep(300);
	}
	WaitForMultipleObjects(threadCnt, hThreadArray, TRUE, INFINITE);
	return 0;
}