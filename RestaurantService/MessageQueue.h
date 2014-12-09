#pragma once

#include <string>

#include "DataSet\MessageQueueData.h"
#include "Config.h"
#include "Json.h"

#include <Shlwapi.h>
#include <fstream>

using namespace std;

class MessageQueue
{
private:
	static MessageQueue *instance;

	bool isUsable();
	void lockFile();
	void unLockFile();
	bool isEmpty(ifstream& in);
	MessageQueue();

public:
	static MessageQueue* getInstance();
	MessageQueueData* Dequeue();
};