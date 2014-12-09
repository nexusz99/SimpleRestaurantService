#include "Json.h"
#include "rapidjson\document.h"
#include "rapidjson\prettywriter.h"

using namespace rapidjson;

MessageType toMessageType(int t);

MessageQueueData* Json::deserialize(string json)
{
	MessageQueueData* d = new MessageQueueData();
	Document doc;
	doc.Parse(json.c_str());

	d->client_id = doc["client_id"].GetString();
	d->msgType = toMessageType(doc["msgType"].GetInt());

	switch(d->msgType)
	{
	case ORDER:
		{
		OrderData *od = new OrderData();
		od->amount = doc["data"]["amount"].GetInt();
		od->itemNo = doc["data"]["itemNo"].GetInt();
		d->data = (Data*)od;
		}
		break;
	case LOGIN:
		{
		Login *l = new Login();
		l->username = doc["data"]["username"].GetString();
		l->password = doc["data"]["password"].GetString();
		d->data = (Data*)l;
		}
		break;
	default:
		break;
	}

	return d;
}

string Json::serialize(Serializable& l)
{
	StringBuffer strbuf;
	PrettyWriter<StringBuffer> writer(strbuf);
	writer.StartObject();
	l.Serialize(writer);
	writer.EndObject();
	return strbuf.GetString();
}

MessageType toMessageType(int t)
{
	switch(t)
	{
	case ORDER:
		return ORDER;
	case CANCEL_ORDER:
		return CANCEL_ORDER;
	case PAY:
		return PAY;
	case LOGIN:
		return LOGIN;
	}
	return ORDER;
}