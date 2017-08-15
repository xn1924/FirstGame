//
//  KeyboardScene.hpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/15.
//
//

#pragma once

#include "cocos2d.h"

class KeyboardScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(KeyboardScene);
    
};

