#include "Magic.h"
#include <fstream>
#include "AnimationManager.h"
USING_NS_CC;
using namespace std;
bool Magic::init()
{
	if (!Sprite::init()) {
		return false;
	}
	return true;
}

Magic* Magic::createMagic(std::string path)
{
	stringstream ss;
	ss << path << "animate.json";
	std::string p = ss.str();
	std::ifstream ifs;
	ifs.open(ss.str());
	assert(ifs.is_open());

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root, false))
	{
		return nullptr;
	}


	auto magic = Magic::create();
	magic->setFlippedY(true);
	magic->path = path;
	magic->id = root["id"].asInt();
	magic->name = root["name"].asString();
	magic->numbers = root["numbers"].asInt();
	magic->startIndex = root["indexes"].asString();
	stringstream s;
	s << path << magic->startIndex << "00.tga";
	magic->initWithFile(s.str());
	return magic;
}


void Magic::play()
{
	auto magic = AnimationManager::createAnimate(path,startIndex, numbers);

	auto after_attacked = CallFunc::create([=]() {
		removeFromParent();
	});
	auto seq = Sequence::create(magic, after_attacked, nullptr);

	runAction(seq);
}