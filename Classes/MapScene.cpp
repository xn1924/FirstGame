#include "MapScene.h"
#include "CombatScene.h"
#include "MainPanel.h"

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
	Size sz = Director::getInstance()->getOpenGLView()->getFrameSize();
	Vec2 center = Vec2(sz.width / 2, sz.height / 2);

	auto map = Sprite::create("mhxy/map/bjluzhou.jpg");
	map->setPosition(center - Vec2(100, 100));
	addChild(map);

	auto npc = Actor::createActor(10000);
	npc->pos_combat = 2;
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
			auto iv = ui::ImageView::create("mhxy/avatar/creatures/1576-e84a1bd5-00000.tga");
			iv->setAnchorPoint(Vec2(0, 1));
			iv->setFlippedY(true);
			iv->setPosition(Vec2(210, 340));
			addChild(iv);

			{
				auto iv = ui::ImageView::create("mhxy/UI/dialog.tga");
				iv->setFlippedY(true);
				iv->setPosition(Vec2(center.x, 260));
				addChild(iv);

				auto lb = Label::createWithTTF("敢撩我，找死啊！", "fonts/simhei.ttf", 14);
				lb->setAnchorPoint(Vec2(0, 0.5));
				lb->getFontAtlas()->setAliasTexParameters();
				lb->setPosition(Vec2(178, 300));
				addChild(lb);

				auto button = ui::Button::create();
				button->setTitleText("怕你不成！");
				button->setTitleColor(Color3B::RED);
				button->setTitleFontSize(12);
				button->setAnchorPoint(Vec2(0, 0.5));
				button->setPosition(Vec2(178, 260));
				addChild(button);

				button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
					switch (type)
					{
					case cocos2d::ui::Widget::TouchEventType::BEGAN:
						break;
					case cocos2d::ui::Widget::TouchEventType::MOVED:
						break;
					case cocos2d::ui::Widget::TouchEventType::ENDED:
						Director::getInstance()->replaceScene(TransitionFade::create(1, CombatScene::createScene()));
						break;
					case cocos2d::ui::Widget::TouchEventType::CANCELED:
						break;
					default:
						break;
					}
				});

				button = ui::Button::create();
				button->setTitleText("大侠饶命，点错了。");
				button->setTitleColor(Color3B::RED);
				button->setTitleFontSize(12);
				button->setAnchorPoint(Vec2(0, 0.5));
				button->setPosition(Vec2(178, 240));
				button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
					switch (type)
					{
					case cocos2d::ui::Widget::TouchEventType::BEGAN:
						break;
					case cocos2d::ui::Widget::TouchEventType::MOVED:
						break;
					case cocos2d::ui::Widget::TouchEventType::ENDED:
						break;
					case cocos2d::ui::Widget::TouchEventType::CANCELED:
						break;
					default:
						break;
					}
				});
				addChild(button);
			}
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

//private
Actor * createActor(int index, cocos2d::Vec2 pos, int standType)
{
	auto actor = Actor::createActor(index);
	actor->setPosition(pos);
	actor->stand(standType);
	return actor;
}


