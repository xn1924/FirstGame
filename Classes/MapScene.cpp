#include "MapScene.h"
#include "MainPanel.h"
#include "NPCDialog.h"
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

USING_NS_CC;
using namespace std;

static Actor * createActor(int index, cocos2d::Vec2 pos, int standType);
static ui::Button* creButton(std::string& name);

Scene* MapScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MapScene::create();
	scene->addChild(layer);
	return scene;
}

bool MapScene::init()
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
		Point mouse = event->getLocation();
		mouse.y = 600 - mouse.y;
		_cursor->setVisible(true);
		_cursor->setPosition(Point(mouse.x + 16, mouse.y - 16));
	};
	listenerMouse->onMouseDown = [&](cocos2d::EventMouse* event) {
		_cursor->removeAllChildren();
		auto cursor = Sprite::create("mhxy/UI/cursor_point_down.png");
		_cursor->addChild(cursor);
	};
	listenerMouse->onMouseUp = [&](cocos2d::EventMouse* event) {
		_cursor->removeAllChildren();
		auto cursor = Sprite::create("mhxy/UI/cursor_nor.png");
		_cursor->addChild(cursor);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);

	Size sz = Director::getInstance()->getVisibleSize();
	Vec2 center = Vec2(sz.width / 2, sz.height / 2);

	auto map = Sprite::create("mhxy/map/bjluzhou.jpg");
	map->setPosition(center - Vec2(100, 100));
	addChild(map);

	auto npc = Actor::createActor(10000);
	npc->setCombatPos(2);
	npc->setPosition(map->getContentSize().width / 2 + 300, map->getContentSize().height / 2 + 300);
	npc->idle();
	map->addChild(npc);

	auto hero = createActor(20000, center , 0);
	addChild(hero);
	
	auto mainPanel = MainPanel::create();
	addChild(mainPanel);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event)->bool {
		Node* target = nullptr;
		auto layer = event->getCurrentTarget();
		for each (auto sprite in layer->getChildren())
		{
			auto bounds = sprite->getBoundingBox();
			Vec2 touchInNode = layer->convertToNodeSpace(touch->getLocation());
			if (bounds.containsPoint(touchInNode)) {
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
		if (target == npc) {
			auto npcDialog = NPCDialog::create();
			addChild(npcDialog);

		}
		else {
			map->stopAllActions();

			Vec2 dir = touch->getLocation() - hero->getPosition();
			auto move_forward = MoveBy::create(dir.length() / 100, dir * -1);
			map->runAction(move_forward);

			hero->walk(touch->getLocation(), false);

		}

		return true;
	};
	
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, map);
	return true;
}
void MapScene::onEnter()
{
	Layer::onEnter();
	_cursor->setVisible(false);
}
//private
Actor * createActor(int index, cocos2d::Vec2 pos, int standType)
{
	auto actor = Actor::createActor(index);
	actor->setPosition(pos);
	actor->stand(standType);
	return actor;
}


