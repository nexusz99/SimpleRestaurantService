#pragma once

#include <iostream>

#include "DataSet\HashTable.h"
#include "DataSet\LinkedList.h"
#include "DataSet\UserData.h"
#include "DataSet\PayData.h"
#include "DataSet\MenuData.h"
#include "Json.h"

using namespace std;

class RestraurantManager
{
public:
	void startup();
	bool verifyLogin(string username, string password);
	bool createNewUser(string username, string password);
	void createNewMenu(string name, int time, int cost);
	void writePaymentInfo(PayData* p);

	string getItemNameById(int id);
	int getCookTimeById(int id);

	HashTable<UserData> userTable;
	LinkedList<PayData*> payList;
	LinkedList<MenuData*> menuList;

private:
	void loadUserList();
	void loadPaymentHistory();
	void loadMenuList();
};