#pragma once
#include "cocos2d.h"
#include <json/json.h>

class Actor : public cocos2d::Sprite
{
public:
	int id;
	std::string name;
	int pos_combat;
	static Actor* createActor(std::string path,int pos_combat);
	virtual bool init();
	CREATE_FUNC(Actor);

	void attack(Actor* target);

	void suffer();

	void defend();

	void idle();

	void dead();
private:
	cocos2d::Animate* createAnimate(const std::vector<std::string>& names, float delay);

	cocos2d::Animate* createAnimate(const std::string& prefix, const int& count, float delay = 0.08f, std::string type = ".tga", bool reverse = false);
	//meta
	std::string path;
	//animations
	Json::Value anim;

	

	

};