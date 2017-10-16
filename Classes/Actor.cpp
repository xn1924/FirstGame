#include "Actor.h"
#include "Magic.h"
#include "ActorParser.h"
#include "ui/CocosGUI.h"
#include "AnimationManager.h"
USING_NS_CC;
using namespace std;
#define TAG_IDLE 10
#define PI 3.1415926535898
int calDirection(Vec2 dir);
Texture2D* snap(Animate* animate);

bool Actor::init()
{
	if (!Sprite::init()) {
		return false;
	}
	return true;
}

Actor* Actor::createActor(int index)
{		
	auto actor = Actor::create();
	actor->_id = index;

	Json::Value root = ActorParser::getValueForActor(index,actor->_path);

	actor->setFlippedY(true);

	actor->_id = root["id"].asInt();
	actor->_name = root["name"].asString();
	actor->_anim = root["combat_anim"];
	actor->_blood = 3000;

	Texture2D* texture = snap(AnimationManager::createAnimate(actor->_path, actor->_anim["idle"]["indexes"][actor->getCombatPos()].asString(), actor->_anim["idle"]["numbers"][actor->getCombatPos()].asInt()));
	Size sz = texture->getContentSize();
	auto bg = ui::ImageView::create("mhxy/UI/combat_blood_bg.png");
	bg->setTag(100);
	bg->setVisible(false);
	bg->setAnchorPoint(Vec2(0.5, 0));
	bg->setPosition(Vec2(sz.width / 2, sz.height));
	actor->addChild(bg);

	auto bar = ui::ImageView::create("mhxy/UI/combat_blood.png");
	bar->setTag(101);
	bar->setPosition(Vec2(3, 1));
	bar->setAnchorPoint(Vec2(0, 0));
	bg->addChild(bar);

	auto value = Label::createWithTTF("400", "fonts/simhei.ttf", 20);
	value->setTag(102);
	value->enableBold();
	value->setVisible(false);
	value->setAnchorPoint(Vec2(0.5, 0));
	value->getFontAtlas()->setAliasTexParameters();
	value->setTextColor(Color4B::RED);
	value->setPosition(Vec2(sz.width / 2, sz.height / 2));
	actor->addChild(value,1);

	return actor;
}

void Actor::setBloodBarDisplay(bool show) {
	auto bar = getChildByTag(100);
	bar->setVisible(show);
}
void Actor::updateBloodBarValue() {
	auto bar = getChildByTag(100);
	auto blood_bar = bar->getChildByTag(101);
	float scale = _blood / 3000.f;
	if (scale < 0)
		scale = 0;
	blood_bar->setScaleX(scale);
}
void Actor::showDamageValues() {
	auto lb = getChildByTag(102);
	lb->setVisible(true);

	Vec2 movement = Vec2(0, 20);
	auto move1 = MoveBy::create(0.1, movement);
	auto move2 = MoveBy::create(0.1, movement*(-1));

	auto delay = DelayTime::create(0.5);

	auto after_delay = CallFunc::create([=]() {
		lb->setVisible(false);
	});

	auto seq = Sequence::create(move1, move2, delay, after_delay, nullptr);
	lb->runAction(seq);
}
#pragma region Animation
void Actor::idle()
{
	auto idle = RepeatForever::create(AnimationManager::createAnimate(_path,_anim["idle"]["indexes"][_combatPos].asString(), _anim["idle"]["numbers"][_combatPos].asInt()));
	idle->setTag(TAG_IDLE);
	runAction(idle);
}
void Actor::suffer(float time)
{
	auto magic = Magic::createMagic("mhxy/magic1/");
	magic->setAnchorPoint(Vec2(0,0));
	Vec2 magic_center = Vec2(magic->getContentSize().width / 2, magic->getContentSize().height / 2);
	Vec2 center = Vec2(getContentSize().width / 2, getContentSize().height / 2);
	magic->setPosition(center - magic_center);
	magic->setVisible(false);
	addChild(magic);

	Vec2 movement = Vec2(_combatPos ==0?20:-20, 0);
	auto move1 = EaseIn::create(MoveBy::create(0.2, movement), 2);
	auto move2 = EaseOut::create(MoveBy::create(0.2, movement*(-1)), 2);

	auto suffer = AnimationManager::createAnimate(_path,_anim["suffer"]["indexes"][_combatPos].asString(), _anim["suffer"]["numbers"][_combatPos].asInt());
	
	auto delay = DelayTime::create(time);

	auto after_delay = CallFunc::create([=]() {
		stopActionByTag(TAG_IDLE);
		magic->play();

		_blood -= 400;
		updateBloodBarValue();
		showDamageValues();
	});

	auto values = getChildByTag(102);
	auto after_attacked = CallFunc::create([=]() {
		if (_blood <= 0)
		{
			magic->setVisible(false);
			values->setVisible(false);
			setBloodBarDisplay(false);
			dead();
		}
		else
			idle();
		
	});

	auto seq = Sequence::create(delay, after_delay, suffer,move1,move2, after_attacked, nullptr);
	runAction(seq);
}
void Actor::defend()
{
	runAction(AnimationManager::createAnimate(_path,_anim["defend"]["indexes"][_combatPos].asString(), _anim["defend"]["numbers"][_combatPos].asInt()));
}
void Actor::dead()
{
	auto dead = AnimationManager::createAnimate(_path, _anim["dead"]["indexes"][_combatPos].asString(), _anim["dead"]["numbers"][_combatPos].asInt());

	auto finish = CallFunc::create([=]() {
		if (deadAction)
			deadAction();
	});

	auto seq = Sequence::create(dead, finish, nullptr);
	runAction(seq);
}

void Actor::stand(int type)
{
	auto stand = RepeatForever::create(AnimationManager::createAnimate(_path,_anim["stand"]["indexes"][type].asString(), _anim["stand"]["numbers"][type].asInt()));

	runAction(stand);
}
void Actor::attack(Actor* target, int type/*, const AttackCallBack& callback*/)
{
	if (target->_blood <= 0)
		return;
	setStatus(CombatStatus::ON_ATTACK);

	stopAllActions();
	//attacker¶¯»­
	Vec2 offsetPos = _combatPos == 0 ? Vec2(110, -40) : Vec2(-90, 50);
	Vec2 targetPos = target->getPosition() + offsetPos;
	Vec2 movement = targetPos - getPosition();

	auto move_forward = EaseIn::create(MoveBy::create(0.3, movement), 1.2);
	auto move_back = EaseIn::create(MoveBy::create(0.3, movement*(-1)), 1.2);

	auto combat_forward = AnimationManager::createAnimate(_path,_anim["forward"]["indexes"][_combatPos].asString(), _anim["forward"]["numbers"][_combatPos].asInt());
	auto combat_back = AnimationManager::createAnimate(_path,_anim["back"]["indexes"][(_combatPos + 2) % 4].asString(), _anim["back"]["numbers"][(_combatPos + 2) % 4].asInt());
	auto combat_action = AnimationManager::createAnimate(_path,_anim["attacks"][type]["indexes"][_combatPos].asString(), _anim["attacks"][type]["numbers"][_combatPos].asInt());

	auto forward_spawn = Spawn::createWithTwoActions(move_forward, combat_forward);
	auto back_spawn = Spawn::createWithTwoActions(move_back, combat_back);
	auto finish = CallFunc::create([=]() {
		idle();
		setStatus(CombatStatus::UNPREPARED);
		//callback();
		if (userAction)
			userAction();
	});
	auto bar = getChildByTag(100);
	bar->runAction(move_forward);

	auto beforeCombat = CallFunc::create([=]() {
		bar->setVisible(false);
	});
	auto afterCombat = CallFunc::create([=]() {
		bar->setVisible(true);
		bar->runAction(move_back);
	});

	auto seq = Sequence::create(forward_spawn, beforeCombat,combat_action, afterCombat,back_spawn,  finish, nullptr);
	runAction(seq);

	//target¶¯»­
	float time = _anim["attacks"][type]["key_time"][_combatPos].asFloat();

	target->suffer(time);

}

void Actor::walk(Vec2 target, bool canMove) {
	stopAllActions();

	Vec2 dir = target - getPosition();
	int type = calDirection(dir);

	auto move_action = RepeatForever::create(AnimationManager::createAnimate(_path,_anim["walk"]["indexes"][type].asString(), _anim["walk"]["numbers"][type].asInt()));
	runAction(move_action);

	auto move_forward = canMove ? (FiniteTimeAction *)MoveBy::create(dir.length() / 100, dir) : (FiniteTimeAction *)DelayTime::create(dir.length() / 100);

	auto finish = CallFunc::create([=]() {
		stopAllActions();
		stand(type);
	});

	auto seq = Sequence::create(move_forward, finish, nullptr);
	runAction(seq);

}
#pragma endregion

#pragma region private
Texture2D* snap(Animate* animate)
{
	Vector<AnimationFrame*> frames = animate->getAnimation()->getFrames();
	AnimationFrame* frame = frames.front();
	return frame->getSpriteFrame()->getTexture();
}
int calDirection(Vec2 dir) {
	int type = 0;
	if (dir.x == 0) {
		type = dir.y > 0 ? 2 : 6;
	}
	else if (dir.y == 0) {
		type = dir.x > 0 ? 4 : 0;
	}
	else if (dir.x > 0 && dir.y > 0) {
		type = fabs(dir.y / dir.x) > tan(PI * 3 / 8) ? 2 : fabs(dir.y / dir.x) > tan(PI / 8) ? 3 : 4;
	}
	else if (dir.x > 0 && dir.y < 0) {
		type = fabs(dir.y / dir.x) > tan(PI * 3 / 8) ? 6 : fabs(dir.y / dir.x) > tan(PI / 8) ? 5 : 4;
	}
	else if (dir.x < 0 && dir.y > 0) {
		type = fabs(dir.y / dir.x) > tan(PI * 3 / 8) ? 2 : fabs(dir.y / dir.x) > tan(PI / 8) ? 1 : 0;
	}
	else if (dir.x < 0 && dir.y < 0) {
		type = fabs(dir.y / dir.x) > tan(PI * 3 / 8) ? 6 : fabs(dir.y / dir.x) > tan(PI / 8) ? 7 : 0;
	}
	return type;
}
#pragma endregion


