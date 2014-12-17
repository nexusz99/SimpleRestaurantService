#pragma once

#include <string>

#include "LinkedList.h"
#include "PayData.h"

using namespace std;

class UserData
{
public:

	UserData()
	{
		payList = new LinkedList<PayData*>();
	}

	string username;
	string password;
	LinkedList<PayData*> *payList;
};