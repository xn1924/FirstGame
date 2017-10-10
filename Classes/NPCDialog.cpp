#include "NPCDialog.h"
#include "ui/CocosGUI.h"
#include "CombatScene.h"

USING_NS_CC;
using namespace std;

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

bool NPCDialog::init()
{
	if (!Layer::init()) {
		return false;
	}
	Size sz = Director::getInstance()->getVisibleSize();
	Vec2 center = Vec2(sz.width / 2, sz.height / 2);

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

		auto lb = Label::createWithTTF("¸ÒÁÃÎÒ£¬ÕÒËÀ°¡£¡", "fonts/simhei.ttf", 14);
		lb->setAnchorPoint(Vec2(0, 0.5));
		lb->getFontAtlas()->setAliasTexParameters();
		lb->setPosition(Vec2(178, 300));
		addChild(lb);

		auto button = ui::Button::create();
		button->setTitleText("ÅÂÄã²»³É£¡");
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
			{
				removeFromParent();
				Director::getInstance()->pushScene(TransitionFade::create(1, CombatScene::createScene()));
			}
			break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
			}
		});

		button = ui::Button::create();
		button->setTitleText("´óÏÀÈÄÃü£¬µã´íÁË¡£");
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
				removeFromParent();
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
			}
		});
		addChild(button);
	}

	return true;
}