//
//  AnimateScene.hpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/15.
//
//

#ifndef AnimateScene_hpp
#define AnimateScene_hpp

class AnimateScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(AnimateScene);
    
};
#endif /* AnimateScene_hpp */
