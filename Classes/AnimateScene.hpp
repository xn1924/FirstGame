//
//  AnimateScene.hpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/15.
//
//

#ifndef AnimateScene_hpp
#define AnimateScene_hpp
#include "cocos2d.h"
#include "Actor.h"

class AnimateScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(AnimateScene);

private:
	 Actor* createActor(cocos2d::Vec2 pos, int pos_combat);

	 void triggerAttack(Actor* attacker, Actor* receiver);
};
#endif /* AnimateScene_hpp */
