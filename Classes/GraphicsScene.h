//
//  GraphicsScene.hpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/14.
//
//

#pragma once

#include "cocos2d.h"

class GraphicsScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GraphicsScene);
};
