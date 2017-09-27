#include "MainPanel.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

ui::Button* creButton(std::string& name)
{
	char plist_name[255];
	char png_name_normal[255];
	char png_name_selected[255];
	char png_name_disable[255];
	sprintf(plist_name, "mhxy/UI/buttons/%s.plist", name.c_str());
	sprintf(png_name_normal, "%s-1.png", name.c_str());
	sprintf(png_name_selected, "%s-2.png", name.c_str());
	sprintf(png_name_disable, "%s-3.png", name.c_str());


	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile(plist_name);
	
	auto button = ui::Button::create(png_name_normal, png_name_selected, png_name_disable, ui::Widget::TextureResType::PLIST);
	return button;
}

bool MainPanel::init()
{
	if (!Layer::init()) {
		return false;
	}
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("mhxy/UI/talk_pre.plist");
	// 
	auto iv = ui::ImageView::create("mhxy/UI/1156-1.png");
	iv->setAnchorPoint(Vec2(0, 0));
	iv->setPosition(Vec2(0, 521));
	addChild(iv);

	//auto lb = Label::createWithSystemFont("北俱泸州", "simhei", 16);
	auto lb = Label::createWithTTF("北俱泸州", "fonts/simhei.ttf", 12);
	lb->getFontAtlas()->setAliasTexParameters();
	lb->setPosition(Vec2(60, 580));
	addChild(lb);

	iv = ui::ImageView::create("mhxy/UI/168-1.png");
	iv->setAnchorPoint(Vec2(0, 0));
	iv->setPosition(Vec2(0, 0));
	addChild(iv, 1);

	

	auto button = ui::Button::create("172-1.png", "172-2.png", "172-1.png", ui::Widget::TextureResType::PLIST);
	button->setAnchorPoint(Vec2(0, 0));
	button->setPosition(Vec2(10, 3));
	addChild(button);

	iv = ui::ImageView::create("mhxy/UI/1135-1.png");
	iv->setAnchorPoint(Vec2(0, 0));
	iv->setScale(1.3);
	iv->setPosition(Vec2(50, 0));
	addChild(iv);

	iv = ui::ImageView::create("mhxy/UI/1134-1.png");
	iv->setAnchorPoint(Vec2(0, 0));
	iv->setScale(1.20);
	iv->setPosition(Vec2(800 - 385, 0));
	addChild(iv);

	iv = ui::ImageView::create("mhxy/UI/cat_head_bg.tga");
	iv->setFlippedY(true);
	iv->setPosition(Vec2(590, 580));
	addChild(iv);

	iv = ui::ImageView::create("mhxy/UI/hero_head_bg.tga");
	iv->setFlippedY(true);
	//iv->setPosition(Vec2(703, 575));
	iv->setAnchorPoint(Vec2(0, 1));
	iv->setPosition(Vec2(680, 550));

	addChild(iv);

	button = ui::Button::create("mhxy/UI/cat_head.tga");
	button->setFlippedY(true);
	button->setPosition(Vec2(590, 580));
	addChild(button);

	button = ui::Button::create("mhxy/UI/hero_head.tga");
	button->setPosition(Vec2(683, 553));
	button->setAnchorPoint(Vec2(0, 0));
	addChild(button);

	std::vector<std::string> pos = { "mhxy/UI/blood_bar.tga","mhxy/UI/magic_bar.tga","mhxy/UI/anger_bar.tga","mhxy/UI/exp_bar.tga" };
	for (int i = 0; i < 3; i++) {
		iv = ui::ImageView::create("mhxy/UI/attr_bar_bg.tga");
		iv->setFlippedY(true);
		iv->setAnchorPoint(Vec2(0, 0));
		iv->setPosition(Vec2(610, 600 - 14 * i));
		addChild(iv);

		//iv = ui::ImageView::create(pos[i]);
		//iv->setFlippedY(true);
		//iv->setAnchorPoint(Vec2(0, 0));
		//iv->setPosition(Vec2(622, 600 - 14 * i - 2));
		//addChild(iv);
	}

	for (int i = 0; i < 4; i++) {
		iv = ui::ImageView::create("mhxy/UI/attr_bar_bg.tga");
		iv->setFlippedY(true);
		iv->setAnchorPoint(Vec2(0, 0));
		iv->setPosition(Vec2(730, 600 - 14 * i));
		addChild(iv);

		iv = ui::ImageView::create(pos[i]);
		iv->setFlippedY(true);
		iv->setAnchorPoint(Vec2(0, 0));
		iv->setPosition(Vec2(743, 600 - 14 * i - 2));
		addChild(iv);
	}

	std::vector<std::string> buttons = { "attack","object","give","trade","group","shengxiao","task","bangpai","kuaijie","friend","action","system" };
	for (int i = 0; i < 12; i++) {
		auto button = creButton(buttons[i]);
		button->setAnchorPoint(Vec2(0, 0));
		button->setPosition(Vec2(410 + 32 * i, 6));
		addChild(button);
	}

	return true;
}