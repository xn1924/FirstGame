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
#define UI_MENU_TAG 30
#define UI_MAIN_TAG 40

static Actor* _lastHighlightActor;


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

	auto cursor = Sprite::create("mhxy/UI/cursor_nor.png");
	_cursor = Node::create();
	_cursor->addChild(cursor);
	addChild(_cursor, 10000);

	auto listenerMouse = EventListenerMouse::create();
	listenerMouse->onMouseMove = [&](cocos2d::EventMouse* event) {
		if (_lastHighlightActor)
		{
			//_lastHighlightActor->setBlendFunc({ CC_BLEND_SRC, CC_BLEND_DST });
			//_lastHighlightActor->setColor(Color3B::WHITE);
			_lastHighlightActor->setOpacity(255);
			_lastHighlightActor = nullptr;
		}
		if (!getChildByTag(UI_MENU_TAG)->isVisible())
		{
			return;
		}
		_cursor->setVisible(true);

		Point mouse = event->getLocation();
		mouse.y = 600 - mouse.y;
		this->_cursor->setPosition(Point(mouse.x + 16, mouse.y - 16));

		Actor* target = getActorAtPosition(mouse);
		if (target && target->getTag() == ENEMY_TAG)
		{
			target->setOpacity(200);
			_lastHighlightActor = target;

			this->_cursor->removeAllChildren();
			Sprite* cursor = Sprite::create("mhxy/UI/cursor_attack.png");
			this->_cursor->addChild(cursor);
		}
		else {
			this->_cursor->removeAllChildren();
			Sprite* cursor = Sprite::create("mhxy/UI/cursor_nor.png");
			this->_cursor->addChild(cursor);
		}
	};
	listenerMouse->onMouseDown = [&](cocos2d::EventMouse* event) {
		this->_cursor->removeAllChildren();
		Sprite* cursor = nullptr;
		Point mouse = event->getLocation();
		Actor* target = getActorAtPosition(mouse);
		if (target && (target->getTag() == ENEMY_TAG))
			cursor = Sprite::create("mhxy/UI/cursor_attack.png");
		else
			cursor = Sprite::create("mhxy/UI/cursor_point_down.png");
		this->_cursor->addChild(cursor);
	};
	listenerMouse->onMouseUp = [&](cocos2d::EventMouse* event) {
		this->_cursor->removeAllChildren();
		auto cursor = Sprite::create("mhxy/UI/cursor_nor.png");
		this->_cursor->addChild(cursor);
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);

	Size sz = Director::getInstance()->getOpenGLView()->getFrameSize();
	Vec2 center = Vec2(sz.width / 2, sz.height / 2);

	auto map = Sprite::create("mhxy/map/bjluzhou.jpg");
	map->setPosition(center - Vec2(100, 100)); 
	map->setColor(Color3B(80, 80, 80));
	addChild(map);

	auto maskLayer = Sprite::create("mhxy/UI/combat_bg.tga");
	maskLayer->setAnchorPoint(Vec2(0, 0));
	maskLayer->setScale(1.25, 1.25);
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
	mainPanel->setTag(UI_MAIN_TAG);
	addChild(mainPanel);

	createMenu();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event)->bool {
		Actor* target = getActorAtPosition(touch->getLocation());
		
		if (target && target->getTag() == ENEMY_TAG)
			run((Actor*)target);
		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}

void CombatScene::createMenu()
{
	Vector<MenuItem*> menuItems;
	string str[9] = { "mhxy/UI/menu_run.png","mhxy/UI/menu_catch.png" ,"mhxy/UI/menu_callback.png" ,"mhxy/UI/menu_call.png" ,
		"mhxy/UI/menu_protect.png" ,"mhxy/UI/menu_defend.png" ,"mhxy/UI/menu_tool.png" ,"mhxy/UI/menu_talent.png" ,"mhxy/UI/menu_magic.png"
	};
	for (int i = 0; i < 9; i++) {
		auto item = MenuItemImage::create(str[i], str[i], CC_CALLBACK_1(CombatScene::menuCallback, this));
		item->setPosition(Vec2(680, 260 + i * 25));
		item->setTag(i);
		menuItems.pushBack(item);
	}
	auto menu = Menu::createWithArray(menuItems);
	menu->setPosition(Point::ZERO);
	menu->setTag(UI_MENU_TAG);
	addChild(menu, 1);
}

void CombatScene::run(Actor* target)
{
	if (!_cursor->isVisible())
	{
		if (_lastHighlightActor)
		{
			_lastHighlightActor->setOpacity(255);
			_lastHighlightActor = nullptr;
		}
		return;

	}
	_cursor->setVisible(false);
	auto menu = getChildByTag(UI_MENU_TAG);
	menu->setVisible(false);

	std::vector<Actor *> enemy_list;
	std::vector<Actor *> hero_list;
    
    auto list = this->getChildren();
    for(auto iter = list.begin();iter < list.end();iter++)
	{
		if ((*iter)->getTag() == HERO_TAG) {
			hero_list.push_back(dynamic_cast<Actor *>(*iter));
		}else if ((*iter)->getTag() == ENEMY_TAG) {
			enemy_list.push_back(dynamic_cast<Actor *>(*iter));
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
		if (hero->getBlood() <= 0) {
			Director::getInstance()->popScene();
		}
	};
	for (int i = 0; i < enemy_list.size(); i++)
	{
		enemy_list[i]->userAction = [=]() {
			if(i != enemy_list.size() - 1)
				enemy_list[i + 1]->attack(hero, 2); 
			else {
				getChildByTag(UI_MENU_TAG)->setVisible(true);
			}
		};
		enemy_list[i]->deadAction = [=]() {
			for (int j = 0; j < enemy_list.size(); j++)
			{
				if (enemy_list[j]->getBlood() > 0)
					return;
			}
			Director::getInstance()->popScene();

		};
	}

	hero->attack(target, 1);

}
void CombatScene::menuCallback(Ref *ref) {
	log("TEST");

}
Actor* CombatScene::getActorAtPosition(Vec2 pos) {
	Actor* target = nullptr;
    auto list = getChildren();
    for (auto iter = list.begin();iter<list.end();iter++)
	{
		auto bounds = (*iter)->getBoundingBox();
		if (bounds.containsPoint(pos)) {
			if (target == nullptr)
			{
				target = dynamic_cast<Actor*>(*iter);
			}
			else {
				Vec2 v1 = pos - (*iter)->getPosition();
				Vec2 v2 = pos - target->getPosition();
				if (v1.getLength() <= v2.getLength())
					target = dynamic_cast<Actor*>(*iter);
			}
		}
	}
	return target;
}
//private
Actor* createActor(int index, cocos2d::Vec2 pos, int pos_combat, float speed)
{
	auto actor = Actor::createActor(index);
	//actor->setAnchorPoint(Vec2(0.5, 0));
	actor->setSpeed(speed);
	actor->setCombatPos(pos_combat);
	actor->setPosition(pos);
	actor->setBloodBarDisplay(true);
	actor->idle();

	return actor;
}
