//
//  AnimateScene.cpp
//  FirstGame
//
//  Created by 徐南 on 2017/8/15.
//
//

#include "CombatScene.h"
#include "MainPanel.h"
#include "base/ccMacros.h"
#include "MapScene.h"

USING_NS_CC;
using namespace std;

static Actor* createActor(int index, cocos2d::Vec2 pos, int pos_combat, float speed);
#define HERO_TAG 10
#define ENEMY_TAG 20

Menu* CombatScene::createMenu()
{
	auto layer = Layer::create();

	Vector<MenuItem*> menuItems;
	string str[9] = { "mhxy/UI/menu_run.png","mhxy/UI/menu_catch.png" ,"mhxy/UI/menu_callback.png" ,"mhxy/UI/menu_call.png" ,
		"mhxy/UI/menu_protect.png" ,"mhxy/UI/menu_defend.png" ,"mhxy/UI/menu_tool.png" ,"mhxy/UI/menu_talent.png" ,"mhxy/UI/menu_magic.png" 
		 };
	for (int i = 0; i < 9; i++) {
		auto item = MenuItemImage::create(str[i], str[i], CC_CALLBACK_1(CombatScene::menuCallback, this));
		item->setPosition(Vec2(0,  i * 25));
		item->setTag(i);
		menuItems.pushBack(item);
	}
	auto menu = Menu::createWithArray(menuItems);
	menu->setPosition(Vec2(680, 260));

	return menu;
}
Scene* CombatScene::createScene()
{
	auto scene = Scene::create();
	auto layer = CombatScene::create();
	scene->addChild(layer);
	return scene;
}

bool CombatScene::init()
{
	if (!Layer::init()) {
		return false;
	}
	Size sz = Director::getInstance()->getOpenGLView()->getFrameSize();
	Vec2 center = Vec2(sz.width / 2, sz.height / 2);

	auto map = Sprite::create("mhxy/map/bjluzhou.jpg");
	map->setPosition(center - Vec2(100, 100)); 
	addChild(map);

	auto maskLayer = LayerColor::create(Color4B(43, 39, 80, 200));
	addChild(maskLayer);

	std::vector<Vec2> pos = { Vec2(110,330),Vec2(170,370),Vec2(230,410),Vec2(290,450),Vec2(350,490) };
	for (auto iter = pos.rbegin(); iter != pos.rend(); ++iter)
	{
		auto sprite_left = createActor(10000, *iter, 2, CCRANDOM_0_1());
		sprite_left->setTag(ENEMY_TAG);
		addChild(sprite_left);
	}

	auto sprite_right = createActor(20000, Vec2(550, 240), 0, 1);
	sprite_right->setTag(HERO_TAG);
	addChild(sprite_right);

	auto mainPanel = MainPanel::create();
	mainPanel->setTag(100);
	addChild(mainPanel);

	addChild(createMenu());

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event)->bool {
		Actor* target = nullptr;
		auto layer = event->getCurrentTarget();
		for each (auto sprite in layer->getChildren())
		{
			auto bounds = sprite->getBoundingBox();
			if (bounds.containsPoint(touch->getLocation())) {
				if (target == nullptr)
				{
					target = dynamic_cast<Actor*>(sprite);
				}
				else {
					Vec2 v1 = touch->getLocation() - sprite->getPosition();
					Vec2 v2 = touch->getLocation() - target->getPosition();
					if (v1.getLength() <= v2.getLength())
						target = dynamic_cast<Actor*>(sprite);
				}
			}
		}
		if (target && target->getTag() == ENEMY_TAG)
			run((Actor*)target);
		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}
void CombatScene::run(Actor* target)
{
	std::vector<Actor *> enemy_list;
	std::vector<Actor *> hero_list;

	for each(auto node in this->getChildren())
	{
		if (node->getTag() == HERO_TAG) {
			hero_list.push_back(dynamic_cast<Actor *>(node));
		}else if (node->getTag() == ENEMY_TAG) {
			enemy_list.push_back(dynamic_cast<Actor *>(node));
		}
	}

	std::sort(enemy_list.begin(), enemy_list.end(), [](Actor *node1, Actor *node2)
	{
		if (node1->getSpeed() > node2->getSpeed())
		{
			return true;
		}
		return false;
	});

	Actor* hero = hero_list[0];

	hero->userAction = [=]() {
		enemy_list[0]->attack(hero, 2);
	};
	hero->deadAction = [=]() {
		if (hero->blood <= 0) {
			//Director::getInstance()->popScene();
		}
	};
	for (int i = 0; i < enemy_list.size(); i++)
	{
		enemy_list[i]->userAction = [=]() {
			if(i != enemy_list.size() - 1)
				enemy_list[i + 1]->attack(hero, 2); 
		};
		enemy_list[i]->deadAction = [=]() {
			for (int j = 0; j < enemy_list.size(); j++)
			{
				if (enemy_list[j]->blood > 0)
					return;
			}
			Director::getInstance()->popScene();

		};
	}

	hero->attack(target, 1);

}
void CombatScene::menuCallback(cocos2d::Ref *ref) {

}
//private
Actor* createActor(int index, cocos2d::Vec2 pos, int pos_combat, float speed)
{
	auto actor = Actor::createActor(index);
	//actor->setAnchorPoint(Vec2(0.5, 0));
	actor->setSpeed(speed);
	actor->pos_combat = pos_combat;
	actor->setPosition(pos);
	actor->setBloodBarDisplay(true);
	actor->idle();

	return actor;
}
