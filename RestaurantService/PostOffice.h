#pragma once

#include <string>

#include "Json.h"

using namespace std;

class Letter : Serializable
{
public:
	string client_id;
	string msg;

	void Serialize(PrettyWriter<StringBuffer>& writer) const
	{
		writer.String("client_id");
		writer.String(client_id.c_str());
		writer.String("msg");
		writer.String(msg.c_str());
	}
};

class CancelOrderMsg : Serializable
{
public:
	bool result;
	string msg;

	void Serialize(PrettyWriter<StringBuffer>& writer) const
	{
		writer.String("result");
		writer.Bool(result);
		writer.String("msg");
		writer.String(msg.c_str());
	}
};

class PostOffice
{
public:
	void sendMessage(Letter l);
};