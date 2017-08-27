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
	std::vector<Vec2> pos = { Vec2(125,200),Vec2(200,275),Vec2(275,350),Vec2(350,425),Vec2(425,500) };

	for (auto iter = pos.rbegin(); iter != pos.rend(); ++iter)
	{
		auto sprite_left = createActor(*iter,2);
		addChild(sprite_left);
	}


	auto sprite_right = createActor(Vec2(700, 250),0);

	addChild(sprite_right);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event)->bool {
		Node* target = nullptr;
		auto layer = event->getCurrentTarget();
		for each (auto sprite in layer->getChildren())
		{
			auto bounds = sprite->getBoundingBox();
			if (bounds.containsPoint(touch->getLocation())) {
				if (target == nullptr)
				{
					target = sprite;
				}
				else {
					Vec2 v1 = touch->getLocation() - sprite->getPosition();
					Vec2 v2 = touch->getLocation() - target->getPosition();
					if (v1.getLength() <= v2.getLength())
						target = sprite;
				}
			}
		}
		if(target)
			triggerAttack(sprite_right, (Actor*)target);

		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}


Actor* AnimateScene::createActor(cocos2d::Vec2 pos ,int pos_combat)
{
	auto actor = Actor::createActor("mhxy/105c/", pos_combat);
	actor->setPosition(pos);
	return actor;
}


void AnimateScene::triggerAttack(Actor* attacker, Actor* target)
{
	//attacker动画
	attacker->attack(target);

}