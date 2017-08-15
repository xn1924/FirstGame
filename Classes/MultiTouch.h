//
//  MultiTouch.hpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/14.
//
//

#pragma once

#include "cocos2d.h"

class MultiTouch : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(MultiTouch);
private:
    const static int MAX_TOUCHES = 5;
    
protected:
    cocos2d::Label* labelTouchLocations[MAX_TOUCHES];
};

