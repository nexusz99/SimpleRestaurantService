#pragma once

#include "rapidjson\prettywriter.h"

using namespace rapidjson;

class Serializable
{
public :
	virtual void Serialize(PrettyWriter<StringBuffer>& writer) const
	{
	}
};