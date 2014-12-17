#pragma once

#include "DataSet\MessageQueueData.h"
#include "Serializable.h"


class Json
{
public:
	MessageQueueData* deserialize(string json);

	string serialize(Serializable& l);
};