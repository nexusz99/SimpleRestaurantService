#include <iostream>
#include <fstream>
#include <Shlwapi.h>
#include <direct.h>

#include "RequestWorker.h"
#include "RestraurantManager.h"
#include "ConsoleLogger.h"
#include "Config.h"

using namespace std;

void initService();
void initConsole();
void printMenu();
void doMenuManagement();
void doUserManagement(); 

RestraurantManager rm;
OrderQueue* oq;

extern CConsoleLoggerEx orderConsole;
extern CConsoleLoggerEx monitorConsole;

int main()
{
	initService();
	initConsole();
	int input;
	while(1)
	{
		system("cls");
		printMenu();
		cin >> input;
		cout << endl;
		switch(input)
		{
		case 1:
			doMenuManagement();
			break;
		case 2:
			doUserManagement();
			break;
		case 3:
			orderConsole.Close();
			monitorConsole.Close();
			exit(1);
			break;
		default:
			
			break;
		}
	}
	return 0;
}

void doMenuManagement()
{
	
	while(true)
	{
		system("cls");
		cout << "==== �Ŵ� ���� ====" << endl;
		cout << "  no  |   �̸�  |  ����  |  ���� �ð� " << endl;
		rm.menuList.ResetIterator();

		MenuData* d;

		try
		{
			while((d = rm.menuList.iter()) != NULL)
			{
				cout << d->id << "      ";
				cout << d->name << "     ";
				cout << d->cost << "     ";
				cout << d->time / 1000 << "��"<<endl;
			}
		}
		catch(int e)
		{
		}

		cout << endl << "-------------------" << endl;
		cout << "1. �Ŵ� �߰�" <<endl;
		cout << "2. ���� �Ŵ���" << endl;
		cout << "�Է� : ";
		int input;
		cin >> input;

		switch(input)
		{
		case 1:
			{
				cout << "�̸� : ";
				string name;
				int time,cost;
				cin >> name;

				cout << "���� : ";
				cin >> cost;

				cout << "���� �ð�(��) : ";
				cin >> time;
				time = time * 1000;

				rm.createNewMenu(name, time, cost);
			}
			break;
		case 2:
			return;
			break;
		default:
			break;
		}
	}
}

void doUserManagement()
{
	UserData **userData = NULL;
	int arr=0;
	while(true)
	{
		system("cls");
		cout << "---- ȸ���� �ŷ� ���� ----" << endl;
		
		LinkedList<char*> *keys = rm.userTable.getKeys();
		keys->ResetIterator();

		char *name;
		int i = 0;
		userData = new UserData*[rm.userTable.getCount()]; 
		while((name = keys->iter())!=NULL)
		{
			userData[i] = new UserData();
			UserData ud = rm.userTable.get(name);
			userData[i]->username = ud.username;
			userData[i]->payList = ud.payList;
			cout << i << " : " << ud.username <<endl;
			i++;
		}
		arr = i;
		

		cout << "ȸ�� ��ȣ : ";
		int input;
		cin >> input;

		LinkedList<PayData*> *plist = userData[input]->payList;
		plist->ResetIterator();
		cout << "����          |    �Ŵ�    |   ���� �ݾ�   |" << endl;
		try
		{
			PayData* p;
			while(true)
			{
				p = plist->iter();
				cout << p->username << "  ";
				cout << p->item << "          ";
				cout << p->cash<<endl;

			}
		}
		catch(int i)
		{
		}
		cout << "�ƹ� ���ڳ� �Է��ϸ�, ó������..";
		cin >> i;
	}
}

void printMenu()
{
	cout << "===== ������ �Ŵ� =====" << endl;
	cout << "1. �Ŵ��� ����" << endl;
	cout << "2. ȸ���� �ŷ� ����" << endl;
	cout << "3. ����" << endl;
	cout << "�Է� : ";

}

void initService()
{
	if(!PathFileExistsA(BASEDIR))
	{
		_mkdir(BASEDIR);		
	}

	if(!PathFileExistsA(USERDIR))
	{
		_mkdir(USERDIR);
	}

	if(!PathFileExistsA(MENUFILE))
	{
		ofstream o(MENUFILE);
		o.close();
	}

	if(!PathFileExistsA(USERFILE))
	{
		ofstream o(USERFILE);
		o.close();
	}

	if(!PathFileExistsA(PAYFILE))
	{
		ofstream o(PAYFILE);
		o.close();
	}

	if(!PathFileExistsA(MESSAGE_QUEUE))
	{
		ofstream o(MESSAGE_QUEUE);
		o.close();
	}

	rm.startup();

	oq = new OrderQueue();

	RequestWorker rw;
	rw.start();
}