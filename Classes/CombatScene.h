//
//  AnimateScene.hpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/15.
//
//

#ifndef CombatScene_hpp
#define CombatScene_hpp
#include "cocos2d.h"
#include "Actor.h"
#include "ui/CocosGUI.h"

class CombatScene : public cocos2d::Layer
{
public:
	bool isRun;
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(CombatScene);
	cocos2d::Menu* createMenu();
	void menuCallback(cocos2d::Ref *ref);
	void run(Actor* target);
};
#endif /* AnimateScene_hpp */
