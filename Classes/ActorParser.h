#pragma once

#ifdef WIN32
#include <json/json.h>
#else
#include "json.h"
#endif
class Actor;
class ActorParser
{
public:
	static Json::Value getValueForActor(unsigned int index, std::string &path);
};
