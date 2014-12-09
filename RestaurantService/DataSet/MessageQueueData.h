#pragma once

#include "../Serializable.h"

#include <string>

enum MessageType{ORDER, CANCEL_ORDER, PAY, LOGIN};

using namespace std;

class Data : Serializable
{
public:
	void Serialize(PrettyWriter<StringBuffer>& writer) const
	{
	}
};

class Login: Data
{
public:
	string username;
	string password;

	void Serialize(PrettyWriter<StringBuffer>& writer) const
	{
		writer.StartObject();
		writer.String("username");
		writer.String(username.c_str());
		writer.String("password");
		writer.String(password.c_str());
	}
};

class OrderData:Data
{
public:
	int itemNo;
	int amount;

	void Serialize(PrettyWriter<StringBuffer>& writer) const
	{
		writer.StartObject();
		writer.String("itemNo");
		writer.Int(itemNo);
		writer.String("amount");
		writer.Int(amount);
		writer.EndObject();
	}
};

class MessageQueueData : Serializable
{

public:
	string client_id;
	MessageType msgType;
	Data *data;

	void Serialize(PrettyWriter<StringBuffer>& writer) const
	{
		writer.String("client_id");
		writer.String(client_id.c_str());
		writer.String("msgType");
		writer.Int(msgType);
		writer.String("data");
		data->Serialize(writer);
	}
};