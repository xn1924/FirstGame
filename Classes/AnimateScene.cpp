//
//  AnimateScene.cpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/15.
//
//

#include "AnimateScene.hpp"

USING_NS_CC;

Scene* AnimateScene::createScene()
{
    auto scene = Scene::create();
    auto layer = AnimateScene::create();
    
    scene->addChild(layer);
    return scene;
}

bool AnimateScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto mySprite = Sprite::create("10000.png");
    mySprite->setPosition(Vec2(mySprite->getContentSize().width/2,mySprite->getContentSize().height/2));
    this->addChild(mySprite);
    
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(8);
    animFrames.pushBack(SpriteFrame::create("10000.png",Rect(0,0,650,600)));
    animFrames.pushBack(SpriteFrame::create("10001.png",Rect(0,0,650,600)));
    animFrames.pushBack(SpriteFrame::create("10002.png",Rect(0,0,650,600)));
    animFrames.pushBack(SpriteFrame::create("10003.png",Rect(0,0,650,600)));
    animFrames.pushBack(SpriteFrame::create("10004.png",Rect(0,0,650,600)));
    animFrames.pushBack(SpriteFrame::create("10005.png",Rect(0,0,650,600)));
    animFrames.pushBack(SpriteFrame::create("10006.png",Rect(0,0,650,600)));
    animFrames.pushBack(SpriteFrame::create("10007.png",Rect(0,0,650,600)));

    Animation* animation = Animation::createWithSpriteFrames(animFrames,0.1);
    Animate* animate = Animate::create(animation);
    
    mySprite->runAction(RepeatForever::create(animate));
    return true;
}
