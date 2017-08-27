#include "Magic.h"
#include <fstream>

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
Animate* Magic::createAnimate(const std::vector<std::string>& names, float delay/*=0.1f*/)
{
	Vector<SpriteFrame*> animFrames;
	for (auto iter = names.cbegin(); iter != names.cend(); iter++) {
		Texture2D *texture = _director->getTextureCache()->addImage(*iter);
		Rect rect = Rect::ZERO;
		rect.size = texture->getContentSize();
		animFrames.pushBack(SpriteFrame::createWithTexture(texture, rect));
	}
	Animation* animation = Animation::createWithSpriteFrames(animFrames, delay);
	Animate* animate = Animate::create(animation);
	return animate;
}
Animate* Magic::createAnimate(const std::string& prefix, const int& count, float delay/*=0.1f*/, std::string type/*=".tga"*/, bool reverse/*=false*/)
{
	std::vector<std::string> names;
	for (int i = 0; i < count; i++)
	{
		int index = !reverse ? i : count - i - 1;
		stringstream ss;
		if (index < 10)
			ss << path << prefix << "0" << index << type;
		else
			ss << path << prefix << index << type;
		names.push_back(ss.str());
	}
	return createAnimate(names, delay);
}

void Magic::play()
{
	auto magic = createAnimate(startIndex, numbers);

	auto after_attacked = CallFunc::create([=]() {
		removeFromParent();
	});
	auto seq = Sequence::create(magic, after_attacked, nullptr);

	runAction(seq);
}