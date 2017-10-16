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
	cocos2d::Node* _cursor;
	bool isRun;
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(CombatScene);
	void createMenu();
	void menuCallback(cocos2d::Ref *ref);
	void run(Actor* target);
	Actor* getActorAtPosition(cocos2d::Vec2 pos);
};
#endif /* AnimateScene_hpp */
