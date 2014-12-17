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
		cout << "==== 매뉴 관리 ====" << endl;
		cout << "  no  |   이름  |  가격  |  조리 시간 " << endl;
		rm.menuList.ResetIterator();

		MenuData* d;

		try
		{
			while((d = rm.menuList.iter()) != NULL)
			{
				cout << d->id << "      ";
				cout << d->name << "     ";
				cout << d->cost << "     ";
				cout << d->time / 1000 << "초"<<endl;
			}
		}
		catch(int e)
		{
		}

		cout << endl << "-------------------" << endl;
		cout << "1. 매뉴 추가" <<endl;
		cout << "2. 메인 매뉴로" << endl;
		cout << "입력 : ";
		int input;
		cin >> input;

		switch(input)
		{
		case 1:
			{
				cout << "이름 : ";
				string name;
				int time,cost;
				cin >> name;

				cout << "가격 : ";
				cin >> cost;

				cout << "조리 시간(초) : ";
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
		cout << "---- 회원별 거래 내역 ----" << endl;
		
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
		

		cout << "회원 번호 : ";
		int input;
		cin >> input;

		LinkedList<PayData*> *plist = userData[input]->payList;
		plist->ResetIterator();
		cout << "고객명          |    매뉴    |   지불 금액   |" << endl;
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
		cout << "아무 숫자나 입력하면, 처음으로..";
		cin >> i;
	}
}

void printMenu()
{
	cout << "===== 관리자 매뉴 =====" << endl;
	cout << "1. 매뉴판 관리" << endl;
	cout << "2. 회원별 거래 내역" << endl;
	cout << "3. 종료" << endl;
	cout << "입력 : ";

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