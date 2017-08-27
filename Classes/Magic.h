#pragma once
#include "cocos2d.h"
#include <json/json.h>

class Magic : public cocos2d::Sprite
{
public:
	int id;
	std::string name;
	static Magic* createMagic(std::string path);
	virtual bool init();
	CREATE_FUNC(Magic);

	void play();

private:
	cocos2d::Animate* createAnimate(const std::vector<std::string>& names, float delay);

	cocos2d::Animate* createAnimate(const std::string& prefix, const int& count, float delay = 0.08f, std::string type = ".tga", bool reverse = false);
	//animations
	std::string path;
	std::string startIndex;
	int numbers;





};