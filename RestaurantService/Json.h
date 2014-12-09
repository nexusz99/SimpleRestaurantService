#pragma once

#include <string>
#include "DataSet\MessageQueueData.h"
#include "Serializable.h"

using namespace std;

class Json
{
public:
	MessageQueueData* deserialize(string json);

	string serialize(Serializable& l);
};