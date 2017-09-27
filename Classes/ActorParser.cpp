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

string getPathForActor(unsigned int index, std::string &path)
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
				stringstream ss; ss << ROOT_PATH << "/" << member[i]<<"/";
				path = ss.str();
				ss << "animate.json";
				return ss.str();
			}
		}
	}
	return "";
}

Json::Value ActorParser::getValueForActor(unsigned int index,std::string &path)
{
	std::string full_path = getPathForActor(index,path);
	Json::Value root = openFile(full_path);

	for (unsigned int i = 0; i < root.size(); i++)
	{
		Json::Value iter = root[i];
		if (iter["id"].asInt() == index) {
			return iter;
		}
	}
	return nullptr;
}

