//
//  KeyboardScene.cpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/15.
//
//

#include "KeyboardScene.h"

USING_NS_CC;

Scene* KeyboardScene::createScene()
{
    auto scene = Scene::create();
    auto layer = KeyboardScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool KeyboardScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    auto sprite = Sprite::create("HelloWorld.png");
    sprite->setPosition(this->getContentSize().width/2,this->getContentSize().height/2);
    this->addChild(sprite,0);
    
    auto eventListener = EventListenerKeyboard::create();
    
    eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){
        Vec2 loc = event->getCurrentTarget()->getPosition();
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case EventKeyboard::KeyCode::KEY_A:
                event->getCurrentTarget()->setPosition(--loc.x,loc.y);
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case EventKeyboard::KeyCode::KEY_D:
                event->getCurrentTarget()->setPosition(++loc.x,loc.y);
                break;
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
            case EventKeyboard::KeyCode::KEY_W:
                event->getCurrentTarget()->setPosition(loc.x,++loc.y);
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            case EventKeyboard::KeyCode::KEY_S:
                event->getCurrentTarget()->setPosition(loc.x,--loc.y);
                break;
            default:
            break;
        }
    };
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, sprite);
    return true;
}
