#include "ActorParser.h"
#include "Actor.h"
#include <fstream>

USING_NS_CC;
using namespace std;

#define ROOT_PATH "mhxy"
Json::Value openFile(string path)
{
	ifstream ifs;
	ifs.open(path);
	assert(ifs.is_open());

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root, false))
	{
		return nullptr;
	}
	return root;
}

string getPathForActor(unsigned int index)
{
	stringstream ss; ss << ROOT_PATH << "/directories.json";
	
	Json::Value root = openFile(ss.str());

	Json::Value::Members member = root.getMemberNames();
	for (unsigned int i = 0; i < member.size(); i++)
	{
		Json::Value indexes = root[member[i]];
		for (unsigned int j = 0; j < indexes.size(); j++)
		{
			if (indexes[j].asInt() == index) {
				return member[i];
			}
		}
	}
	return "";
}
void ActorParser::load(Actor* actor)
{
	stringstream ss; ss << ROOT_PATH <<"/"<< getPathForActor(actor->id)<<"/animate.json";

	Json::Value root = openFile(ss.str());

	actor->path = ss.str();
	actor->id = root["id"].asInt();
	actor->name = root["name"].asString();
	actor->anim = root["combat_anim"];
}