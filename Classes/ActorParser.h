#pragma once
#include <json/json.h>

class Actor;
class ActorParser
{
public:
	static Json::Value getValueForActor(unsigned int index, std::string &path);
};