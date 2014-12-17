#include "MessageQueue.h"

MessageQueue* MessageQueue::instance = NULL;

MessageQueue::MessageQueue()
{
}


MessageQueue* MessageQueue::getInstance()
{
	if(instance == NULL)
	{
		instance = new MessageQueue();
	}
	return instance;
}

MessageQueueData* MessageQueue::Dequeue()
{
	MessageQueueData *data = NULL;
	if(!isUsable())
	{
		return NULL;
	}

	lockFile();
	string raw;
	ifstream in(MESSAGE_QUEUE);
	if(isEmpty(in))
	{
		unLockFile();
		return NULL;
	}
	ofstream out(MESSAGE_QUEUE_TEMP);
	getline(in, raw);
	Json json;
	data = json.deserialize(raw);

	out << in.rdbuf();
	out.close();
	in.close();

	DeleteFileA(MESSAGE_QUEUE);
	rename(MESSAGE_QUEUE_TEMP, MESSAGE_QUEUE);

	unLockFile();

	return data;
}


bool MessageQueue::isUsable()
{
	Sleep(400);
	if(PathFileExistsA(MESSAGE_QUEUE_LOCK))
	{
		return false;
	}
	return true;
}

void MessageQueue::lockFile()
{
	ofstream lckfile(MESSAGE_QUEUE_LOCK);
	lckfile.close();
}

void MessageQueue::unLockFile()
{
	DeleteFileA(MESSAGE_QUEUE_LOCK);
}

bool MessageQueue::isEmpty(ifstream& in)
{
	in.seekg(0, ifstream::end);
	int size = in.tellg();
	in.seekg(0, ifstream::beg);
	if(size == 0)
		return true;
	return false;
}

void MessageQueue::Enqueue(MessageQueueData d)
{
	while(!isUsable())
	{
		Sleep(1000);
	}

	lockFile();

	Json j;
	string str= j.serialize((Serializable&)d);

	ofstream o(MESSAGE_QUEUE, ios::app);

	o.seekp(0, ifstream::end);
	int size = o.tellp();
	if(size != 0)
		o << endl;
	
	o.seekp(0, ifstream::end);
	
	o << str;
	o.close();
	unLockFile();
}