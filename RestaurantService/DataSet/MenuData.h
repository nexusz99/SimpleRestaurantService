#pragma once

#include <string>
#include "../Serializable.h"

using namespace std;

class MenuData : Serializable
{
public:
	int id;
	string name;
	int time;
	int cost;

	bool operator==(MenuData m)
	{
		if(id==m.id)
			return true;
		return false;
	}

	void Serialize(PrettyWriter<StringBuffer>& writer) const
	{
		writer.String("id");
		writer.Int(id);
		writer.String("name");
		writer.String(name.c_str());
		writer.String("time");
		writer.Int(time);
		writer.String("cost");
		writer.Int(cost);
	}
};