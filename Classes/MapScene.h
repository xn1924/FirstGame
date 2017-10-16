#pragma once
#include "cocos2d.h"
#include "Actor.h"
#include "ui/CocosGUI.h"

class MapScene : public cocos2d::Layer
{
public:
	cocos2d::Node* _cursor;

	static cocos2d::Scene* createScene();

	virtual bool init();
	CREATE_FUNC(MapScene);
	virtual void onEnter();
	void setupUIs();
	
};