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
	cout << "===== ������ �Ŵ� =====" << endl;
	cout << "1. �ǽð� ���� ����" << endl;
	cout << "2. ȸ�� ����" << endl;
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

	if(!PathFileExistsA(MESSAGE_QUEUE))
	{
		ofstream o(MESSAGE_QUEUE);
		o.close();
	}

	OrderQueue* oq = new OrderQueue();

	RequestWorker rw(oq);
	rw.start();
}