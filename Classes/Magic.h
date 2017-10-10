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
	//animations
	std::string path;
	std::string startIndex;
	int numbers;





};