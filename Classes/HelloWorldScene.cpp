#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    
    scene->addChild(layer);

    return scene;
}
void HelloWorld::touchEvent(Ref *sender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		log("begin");
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		log("moved");

		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		log("ended");

		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //auto label = Label::createWithSystemFont("Hello World", "Arial", 96);
    //label->setAnchorPoint(Vec2(0.0,0.0));
    //this->addChild(label,1);
    
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto button = ui::Button::create("CloseNormal.png", "CloseSelected.png");
	button->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2+origin.y));
	this->addChild(button);

	button->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEvent, this));

    return true;
}

