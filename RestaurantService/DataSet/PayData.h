#pragma once

#include <string>
#include "../Json.h"

using namespace std;

class PayData : Serializable
{
public:
	string username;
	string item;
	int amount;
	int cash;

	bool operator==(PayData d)
	{
		if(username == d.username && item == d.item && amount == d.amount && d.cash == cash)
			return true;
		return false;
	}

	void Serialize(PrettyWriter<StringBuffer>& writer) const
	{
		writer.String("username");
		writer.String(username.c_str());
		writer.String("item");
		writer.String(item.c_str());
		writer.String("amount");
		writer.Int(amount);
		writer.String("cash");
		writer.Int(cash);
	}
};