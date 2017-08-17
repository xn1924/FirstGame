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
class AnimateScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(AnimateScene);

private:
	 cocos2d::Sprite* createSprite(cocos2d::Vec2 pos,bool flip=false);
	 cocos2d::Animate* createAnimate(const std::vector<std::string>& names,float delay);
	 cocos2d::Animate* createAnimate(const std::string& prefix, const int& count,float delay=0.1f,std::string type=".png",bool reverse=false);

	 void triggerAttack(cocos2d::Sprite* attacker, cocos2d::Sprite* receiver);
};
#endif /* AnimateScene_hpp */
