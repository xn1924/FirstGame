#include "AnimationManager.h"

USING_NS_CC;
using namespace std;

AnimationManager::AnimationManager()
{
}


AnimationManager::~AnimationManager()
{
}

Animate* AnimationManager::createAnimate(const std::vector<std::string>& names, float delay/*=0.1f*/)
{
	Vector<SpriteFrame*> animFrames;
	for (auto iter = names.cbegin(); iter != names.cend(); iter++) {
		Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(*iter);
		//texture->setAliasTexParameters();
		Rect rect = Rect::ZERO;
		rect.size = texture->getContentSize();
		animFrames.pushBack(SpriteFrame::createWithTexture(texture, rect));
	}
	Animation* animation = Animation::createWithSpriteFrames(animFrames, delay);
	Animate* animate = Animate::create(animation);
	return animate;
}
Animate* AnimationManager::createAnimate(const std::string& path, const std::string& prefix, const int& count, float delay/*=0.1f*/, std::string type/*=".tga"*/, bool reverse/*=false*/)
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
