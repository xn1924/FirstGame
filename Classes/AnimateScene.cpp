//
//  AnimateScene.cpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/15.
//
//

#include "AnimateScene.hpp"

USING_NS_CC;
using namespace std;

bool AnimateScene::init()
{
	if (!Layer::init()) {
		return false;
	}
	std::vector<Vec2> pos = { Vec2(125,200),Vec2(200,275),Vec2(275,350),Vec2(350,425),Vec2(425,500) };

	for (auto iter = pos.rbegin(); iter != pos.rend(); ++iter)
	{
		auto sprite_left = createSprite(*iter);
		addChild(sprite_left);
	}


	auto sprite_right = Sprite::create("/sprite/鬼将/01047.png");
	sprite_right->setPosition(Vec2(700, 250));

	auto stand_frame = createAnimate("/sprite/鬼将/0104", 8);
	sprite_right->runAction(RepeatForever::create(stand_frame));

	addChild(sprite_right);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event)->bool {

		auto layer = event->getCurrentTarget();
		for each (auto sprite in layer->getChildren())
		{
			auto bounds = sprite->getBoundingBox();
			if (bounds.containsPoint(touch->getLocation())) {
				triggerAttack(sprite_right, (Sprite*)sprite);
				break;
			}
		}

		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}

Scene* AnimateScene::createScene()
{
    auto scene = Scene::create();
    auto layer = AnimateScene::create();
    
    scene->addChild(layer);
    return scene;
}
Sprite* AnimateScene::createSprite(Vec2 pos, bool flip/*=false*/)
{
	auto sprite = Sprite::create("/sprite/鬼将/01050.png");
	sprite->setPosition(pos);
	sprite->setFlippedX(flip);

	auto stand_frame = RepeatForever::create(createAnimate("/sprite/鬼将/0105", 9));
	stand_frame->setTag(10);
	sprite->runAction(stand_frame);
	return sprite;
}

Animate* AnimateScene::createAnimate(const std::vector<std::string>& names,float delay/*=0.1f*/)
{
	Vector<SpriteFrame*> animFrames;
	for (auto iter = names.cbegin(); iter!=names.cend(); iter++) {
		Texture2D *texture = _director->getTextureCache()->addImage(*iter);
		Rect rect = Rect::ZERO;
		rect.size = texture->getContentSize();
		animFrames.pushBack(SpriteFrame::createWithTexture(texture, rect));
	}
	Animation* animation = Animation::createWithSpriteFrames(animFrames, delay);
	Animate* animate = Animate::create(animation);
	return animate;
}
Animate* AnimateScene::createAnimate(const std::string& prefix, const int& count,float delay/*=0.1f*/, std::string type/*=".png"*/,bool reverse/*=false*/)
{
	std::vector<std::string> names;
	for (int i = 0; i < count; i++)
	{
		int index = !reverse ? i : count - i - 1;
		stringstream ss;
		ss << prefix << index << type;
		names.push_back(ss.str());
	}
	return createAnimate(names,delay);
}

void AnimateScene::triggerAttack(Sprite* attacker,Sprite* target)
{
	//attacker动画
	Vec2 movement = target->getPosition() - Vec2(0, 35) - (attacker->getPosition() - Vec2(100, 0));
	auto forward_frame = createAnimate("/sprite/鬼将/0101", 4);
	auto forward_trans = EaseIn::create(MoveBy::create(0.5, movement),1.2);
	auto forward_spawn = Spawn::createWithTwoActions(forward_trans, forward_frame);

	auto back_frame = createAnimate("/sprite/鬼将/0103", 6);
	auto back_trans = EaseIn::create(MoveBy::create(0.7, movement*(-1)),1.2);
	auto back_spawn = Spawn::createWithTwoActions(back_trans, back_frame);

	auto attack_frame = createAnimate("/sprite/鬼将/0102", 11);
	auto finish = CallFunc::create([=]() {
		auto stand_frame = createAnimate("/sprite/鬼将/0104", 8);
		attacker->runAction(RepeatForever::create(stand_frame));
	});
	auto seq = Sequence::create(forward_spawn, attack_frame, back_spawn, finish, nullptr);
	attacker->stopAllActions();
	attacker->runAction(seq);

	//target动画
	auto delay = DelayTime::create(1.3);
	auto after_delay = CallFunc::create([=]() {
		target->stopActionByTag(10);
	});
	auto attacked = createAnimate("/sprite/鬼将/0106", 2,0.15f);
	auto after_attacked = CallFunc::create([=]() {
		auto stand_frame = RepeatForever::create(createAnimate("/sprite/鬼将/0105", 9));
		stand_frame->setTag(10);
		target->runAction(stand_frame);
	});
	seq = Sequence::create(delay, after_delay,attacked, after_attacked, nullptr);
	target->runAction(seq);

}