#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    
    scene->addChild(layer);
    
    return scene;
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
    
    auto label = Label::createWithSystemFont("Hello World", "Arial", 96);
    label->setAnchorPoint(Vec2(0.0,0.0));
    this->addChild(label,1);
    
    return true;
}

