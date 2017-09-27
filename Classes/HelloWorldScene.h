#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void touchEvent(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);
};

