#pragma once
#include "cocos2d.h"

class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

	static cocos2d::Animate* createAnimate(const std::vector<std::string>& names, float delay);

	static cocos2d::Animate* createAnimate(const std::string& path, const std::string& prefix, const int& count, float delay = 0.08f, std::string type = ".tga", bool reverse = false);
};

