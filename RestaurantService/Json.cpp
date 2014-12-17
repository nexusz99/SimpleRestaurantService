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
	case CANCEL_ORDER:
	case ORDER:
		{
		OrderData *od = new OrderData();
		od->amount = doc["data"]["amount"].GetInt();
		od->itemNo = doc["data"]["itemNo"].GetInt();
		d->data = (Data*)od;
		}
		break;
	case SIGNUP:
	case LOGIN:
		{
		User *l = new User();
		l->username = doc["data"]["username"].GetString();
		l->password = doc["data"]["password"].GetString();
		d->data = (Data*)l;
		}
		break;
	case PAY:
		{
			PayRequest *p = new PayRequest();
			p->amount = doc["data"]["amount"].GetInt();
			p->cost = doc["data"]["cost"].GetInt();
			p->itemno = doc["data"]["itemno"].GetInt();
			d->data = (Data*)p;
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
		break;
	case CANCEL_ORDER:
		return CANCEL_ORDER;
		break;
	case PAY:
		return PAY;
		break;
	case LOGIN:
		return LOGIN;
		break;
	case SIGNUP:
		return SIGNUP;
		break;
	}
	return ORDER;
}