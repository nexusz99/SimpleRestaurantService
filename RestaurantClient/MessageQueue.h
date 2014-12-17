#pragma once

#include <iostream>

#include "../RestaurantService/DataSet/MessageQueueData.h"
#include "../RestaurantService/Config.h"
#include "../RestaurantService/Json.h"

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
	void Enqueue(MessageQueueData d);
};