#include "ConsoleLogger.h"
#include "RestraurantManager.h"
#include "PostOffice.h"
#include "DataSet\OrderQueue.h"

#include <iostream>

using namespace std;

CConsoleLoggerEx orderConsole;
CConsoleLoggerEx monitorConsole;

extern RestraurantManager rm;
extern OrderQueue* oq;

MessageQueueData *current;

DWORD WINAPI cooking(LPVOID lpParameter)
{
	while(true)
	{
		MessageQueueData* d;
		try
		{
			d = oq->Dequeue();
			current = d;
		}
		catch(int i)
		{
			Sleep(100);
			current = NULL;
			continue;
		}
		OrderData* od = (OrderData*)d->data;
		Sleep(rm.getCookTimeById(od->itemNo));

		PostOffice officer;
		Letter l;
		l.client_id = d->client_id;
		l.msg = "1";
		officer.sendMessage(l);
		
	}
}

DWORD WINAPI monitorOrder(LPVOID lpParameter)
{
	while(true)
	{
		
		orderConsole.printf("----------------\n");
		orderConsole.printf("�ǽð� �ֹ� ��Ȳ\n");
		orderConsole.printf("----------------\n\n");
		orderConsole.printf("<< ���� �� >>\n");
		MessageQueueData* d = current;
		
		if(d == NULL)
		{
			Sleep(100);
			orderConsole.cls();
			continue;
		}
		
		OrderData* od = (OrderData*)d->data;
		orderConsole.printf("|   ��  |    �Ŵ�    |  ����  |\n");
		orderConsole.printf("%s       ", d->client_id.c_str());
		orderConsole.printf("%s       ", rm.getItemNameById(od->itemNo).c_str());
		orderConsole.printf("%d   \n\n", od->amount);
		
		orderConsole.printf("<< ��� ��� >>\n");


		oq->print();
		Sleep(400);
		orderConsole.cls();
	}
}

DWORD WINAPI monitorPay(LPVOID lpParameter)
{
	while(true)
	{
		monitorConsole.cls();
		monitorConsole.printf("----- �ǽð� ���� ���� -----\n");
		monitorConsole.printf("����          |    �Ŵ�    |   ���� �ݾ�   |\n");
		
		LinkedList<PayData*> l;
		l = rm.payList;
		
		if(l.getCount()==0)
		{
			Sleep(1000);
			continue;
		}
		l.ResetIterator();
		PayData* tmp;
		try
		{
			while((tmp=l.iter())!=NULL)
			{
				monitorConsole.printf("%s  %s          %d\n",tmp->username.c_str(), tmp->item.c_str(), tmp->cash);
			}
		}
		catch(int i)
		{
			Sleep(1000);
		}
	}


	return 0;
}


void initConsole()
{
	orderConsole.Create("�ֹ� ��Ȳ��");
	monitorConsole.Create("�ǽð� ��������");
	CreateThread(NULL, 0, monitorOrder, NULL, 0, 0);
	CreateThread(NULL, 0, cooking, NULL, 0, 0);
	CreateThread(NULL, 0, monitorPay, NULL, 0, 0);
}