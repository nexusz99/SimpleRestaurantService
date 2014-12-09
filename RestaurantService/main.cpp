#include <iostream>
#include <fstream>
#include <string>	
#include <Shlwapi.h>
#include <direct.h>

#include "RequestWorker.h"
#include "Config.h"

using namespace std;

void initService();
void printMenu();

int main()
{
	initService();
	return 0;
}

void printMenu()
{
	cout << "===== 관리자 매뉴 =====" << endl;
	cout << "1. 실시간 정산 내역" << endl;
	cout << "2. 회원 관리" << endl;
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

	if(!PathFileExistsA(MESSAGE_QUEUE))
	{
		ofstream o(MESSAGE_QUEUE);
		o.close();
	}

	OrderQueue* oq = new OrderQueue();

	RequestWorker rw(oq);
	rw.start();
}