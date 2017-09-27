#include "Actor.h"
#include "Magic.h"
#include "ActorParser.h"
#include <fstream>
USING_NS_CC;
using namespace std;
#define TAG_IDLE 10

int calDirection(Vec2 dir);

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
	actor->id = index;

	Json::Value root = ActorParser::getValueForActor(index,actor->path);

	actor->setFlippedY(true);

	actor->id = root["id"].asInt();
	actor->name = root["name"].asString();
	actor->anim = root["combat_anim"];

	return actor;
}
Animate* Actor::createAnimate(const std::vector<std::string>& names, float delay/*=0.1f*/)
{
	Vector<SpriteFrame*> animFrames;
	for (auto iter = names.cbegin(); iter != names.cend(); iter++) {
		Texture2D *texture = _director->getTextureCache()->addImage(*iter);
		//texture->setAliasTexParameters();
		Rect rect = Rect::ZERO;
		rect.size = texture->getContentSize();
		animFrames.pushBack(SpriteFrame::createWithTexture(texture, rect));
	}
	Animation* animation = Animation::createWithSpriteFrames(animFrames, delay);
	Animate* animate = Animate::create(animation);
	return animate;
}
Animate* Actor::createAnimate(const std::string& prefix, const int& count, float delay/*=0.1f*/, std::string type/*=".tga"*/, bool reverse/*=false*/)
{
	std::vector<std::string> names;
	for (int i = 0; i < count; i++)
	{
		int index = !reverse ? i : count - i - 1;
		stringstream ss;
		if(index < 10)
			ss << path << prefix <<"0"<< index << type;
		else
			ss << path << prefix << index << type;
		names.push_back(ss.str());
	}
	//if (!getTexture())
	//{
		setTexture(names[0]);
	//}
	return createAnimate(names, delay);
}

void Actor::idle()
{
	auto idle = RepeatForever::create(createAnimate(anim["idle"]["indexes"][pos_combat].asString(), anim["idle"]["numbers"][pos_combat].asInt()));
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
	addChild(magic);

	Vec2 movement = Vec2(pos_combat==0?20:-20, 0);
	auto move1 = EaseIn::create(MoveBy::create(0.2, movement), 2);
	auto move2 = EaseOut::create(MoveBy::create(0.2, movement*(-1)), 2);

	auto suffer = createAnimate(anim["suffer"]["indexes"][pos_combat].asString(), anim["suffer"]["numbers"][pos_combat].asInt());

	auto delay = DelayTime::create(time);

	auto after_delay = CallFunc::create([=]() {
		stopActionByTag(TAG_IDLE);
		magic->play();
	});
	auto after_attacked = CallFunc::create([=]() {
		idle();
		
	});

	auto seq = Sequence::create(delay, after_delay, suffer,move1,move2, after_attacked, nullptr);
	runAction(seq);
}
void Actor::defend()
{
	runAction(createAnimate(anim["defend"]["indexes"][pos_combat].asString(), anim["defend"]["numbers"][pos_combat].asInt()));
}
void Actor::dead()
{
	runAction(createAnimate(anim["dead"]["indexes"][pos_combat].asString(), anim["dead"]["numbers"][pos_combat].asInt()));
}

void Actor::stand(int type)
{
	auto stand = RepeatForever::create(createAnimate(anim["stand"]["indexes"][type].asString(), anim["stand"]["numbers"][type].asInt()));

	runAction(stand);
}
void Actor::attack(Actor* target, int type, const AttackCallBack& callback)
{
	setStatus(CombatStatus::ON_ATTACK);

	stopAllActions();
	//attacker¶¯»­
	Vec2 offsetPos = pos_combat == 0 ? Vec2(110, -40) : Vec2(-90, 50);
	Vec2 targetPos = target->getPosition() + offsetPos;
	Vec2 movement = targetPos - getPosition();
	//Vec2 movement = target->getPosition() - getPosition();
	//movement = movement.getNormalized()*(movement.getLength() - 100);

	auto move_forward = EaseIn::create(MoveBy::create(0.3, movement), 1.2);
	auto move_back = EaseIn::create(MoveBy::create(0.3, movement*(-1)), 1.2);

	auto combat_forward = createAnimate(anim["forward"]["indexes"][pos_combat].asString(), anim["forward"]["numbers"][pos_combat].asInt());
	auto combat_back = createAnimate(anim["back"]["indexes"][(pos_combat + 2) % 4].asString(), anim["back"]["numbers"][(pos_combat + 2) % 4].asInt());
	auto combat_action = createAnimate(anim["attacks"][type]["indexes"][pos_combat].asString(), anim["attacks"][type]["numbers"][pos_combat].asInt());

	auto forward_spawn = Spawn::createWithTwoActions(move_forward, combat_forward);
	auto back_spawn = Spawn::createWithTwoActions(move_back, combat_back);
	auto finish = CallFunc::create([=]() {
		idle();
		setStatus(CombatStatus::UNPREPARED);
		callback();
		//target->attack(this, 1);
	});

	auto test = CallFunc::create([=]() {
		Size sz = getContentSize();
		auto bar = getChildByTag(100);
		bar->setPosition(Vec2(sz.width / 2 + 50, sz.height + 20));
	});
	auto test1 = CallFunc::create([=]() {
		Size sz = getContentSize();
		auto bar = getChildByTag(100);
		bar->setPosition(Vec2(sz.width / 2 , sz.height));
	});
	auto seq = Sequence::create(forward_spawn, test,combat_action, back_spawn,  finish, test1, nullptr);
	runAction(seq);

	//target¶¯»­
	float time = anim["attacks"][type]["key_time"][pos_combat].asFloat();
	target->suffer(time);
}

void Actor::walk(Vec2 target, bool canMove) {
	stopAllActions();

	Vec2 dir = target - getPosition();
	int type = calDirection(dir);

	auto move_action = RepeatForever::create(createAnimate(anim["walk"]["indexes"][type].asString(), anim["walk"]["numbers"][type].asInt()));
	runAction(move_action);

	auto move_forward = canMove ? (FiniteTimeAction *)MoveBy::create(dir.length() / 100, dir) : (FiniteTimeAction *)DelayTime::create(dir.length() / 100);

	auto finish = CallFunc::create([=]() {
		stopAllActions();
		stand(type);
	});

	auto seq = Sequence::create(move_forward, finish, nullptr);
	runAction(seq);

}

//private
int calDirection(Vec2 dir) {
	int type = 0;
	if (dir.x == 0) {
		type = dir.y > 0 ? 2 : 6;
	}
	else if (dir.y == 0) {
		type = dir.x > 0 ? 4 : 0;
	}
	else if (dir.x > 0 && dir.y > 0) {
		type = fabs(dir.y / dir.x) > tan(std::_Pi * 3 / 8) ? 2 : fabs(dir.y / dir.x) > tan(std::_Pi / 8) ? 3 : 4;
	}
	else if (dir.x > 0 && dir.y < 0) {
		type = fabs(dir.y / dir.x) > tan(std::_Pi * 3 / 8) ? 6 : fabs(dir.y / dir.x) > tan(std::_Pi / 8) ? 5 : 4;
	}
	else if (dir.x < 0 && dir.y > 0) {
		type = fabs(dir.y / dir.x) > tan(std::_Pi * 3 / 8) ? 2 : fabs(dir.y / dir.x) > tan(std::_Pi / 8) ? 1 : 0;
	}
	else if (dir.x < 0 && dir.y < 0) {
		type = fabs(dir.y / dir.x) > tan(std::_Pi * 3 / 8) ? 6 : fabs(dir.y / dir.x) > tan(std::_Pi / 8) ? 7 : 0;
	}
	return type;
}