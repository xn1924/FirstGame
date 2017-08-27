//
//  MultiTouch.cpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/14.
//
//

#include "MultiTouch.h"

USING_NS_CC;

Scene* MultiTouch::createScene()
{
    auto scene = Scene::create();
    auto layer = MultiTouch::create();
    scene->addChild(layer);
    
    return scene;
}

bool MultiTouch::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    for (int i = 0; i < MAX_TOUCHES; ++i) {
        labelTouchLocations[i] = Label::createWithSystemFont("", "Arial", 42);
        labelTouchLocations[i]->setVisible(false);
        this->addChild(labelTouchLocations[i]);
    }
    
    auto eventListener = EventListenerTouchAllAtOnce::create();
    
    eventListener->onTouchesBegan = [=](const std::vector<Touch*>&touches, Event* event){
        
        std::for_each(labelTouchLocations, labelTouchLocations+MAX_TOUCHES, [](Label* touchLabel){
            touchLabel->setVisible(false);
        });
        
        for ( int i = 0; i < touches.size(); ++i) {
            labelTouchLocations[i]->setPosition(touches[i]->getLocation());
            labelTouchLocations[i]->setVisible(true);
            labelTouchLocations[i]->setString("Touched");
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    return true;
}
