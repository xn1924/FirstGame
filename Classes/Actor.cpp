#include "Actor.h"
#include "Magic.h"

#include <fstream>
USING_NS_CC;
using namespace std;
#define TAG_IDLE 10
bool Actor::init()
{
	if (!Sprite::init()) {
		return false;
	}
	return true;
}

Actor* Actor::createActor(std::string path, int pos_combat)
{
	stringstream ss;
	ss << path << "animate.json";
	std::string p = ss.str();
	std::ifstream ifs;
	ifs.open(ss.str());
	assert(ifs.is_open());

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root, false))
	{
		return nullptr;
	}
	
		
	auto actor = Actor::create();
	actor->setFlippedY(true);
	actor->path = path;
	actor->pos_combat = pos_combat;
	actor->id = root["id"].asInt();
	actor->name = root["name"].asString();
	actor->anim = root["combat_anim"];

	actor->idle();
	return actor;
}
Animate* Actor::createAnimate(const std::vector<std::string>& names, float delay/*=0.1f*/)
{
	Vector<SpriteFrame*> animFrames;
	for (auto iter = names.cbegin(); iter != names.cend(); iter++) {
		Texture2D *texture = _director->getTextureCache()->addImage(*iter);
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
	return createAnimate(names, delay);
}

void Actor::idle()
{
	auto idle = RepeatForever::create(createAnimate(anim["idle"]["indexes"][pos_combat].asString(), anim["idle"]["numbers"][pos_combat].asInt()));
	idle->setTag(TAG_IDLE);
	runAction(idle);
}
void Actor::suffer()
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

	auto delay = DelayTime::create(1.3);

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
void Actor::attack(Actor* target)
{

	stopAllActions();
	//attacker¶¯»­

	Vec2 movement = target->getPosition() - Vec2(0, 25) - (getPosition() - Vec2(90, 0));
	//Vec2 movement = target->getPosition() - getPosition();
	//movement = movement.getNormalized()*(movement.getLength() - 100);

	auto move_forward = EaseIn::create(MoveBy::create(0.3, movement), 1.2);
	auto move_back = EaseIn::create(MoveBy::create(0.3, movement*(-1)), 1.2);

	auto combat_forward = createAnimate(anim["forward"]["indexes"][pos_combat].asString(), anim["forward"]["numbers"][pos_combat].asInt());
	auto combat_back = createAnimate(anim["back"]["indexes"][(pos_combat + 2) % 4].asString(), anim["back"]["numbers"][(pos_combat + 2) % 4].asInt());
	auto combat_action = createAnimate(anim["attacks"][2]["indexes"][pos_combat].asString(), anim["attacks"][2]["numbers"][pos_combat].asInt());


	auto forward_spawn = Spawn::createWithTwoActions(move_forward, combat_forward);
	auto back_spawn = Spawn::createWithTwoActions(move_back, combat_back);
	auto finish = CallFunc::create([=]() {
		idle();
	});

	auto seq = Sequence::create(forward_spawn, combat_action, back_spawn, finish, nullptr);
	runAction(seq);

	//target¶¯»­
	target->suffer();
}

//actor->forward_0 = actor->createAnimate(anim["forward"]["indexes"][0].asString(), anim["forward"]["numbers"][0].asInt());
//actor->forward_1 = actor->createAnimate(anim["forward"]["indexes"][1].asString(), anim["forward"]["numbers"][1].asInt());
//actor->forward_2 = actor->createAnimate(anim["forward"]["indexes"][2].asString(), anim["forward"]["numbers"][2].asInt());
//actor->forward_3 = actor->createAnimate(anim["forward"]["indexes"][3].asString(), anim["forward"]["numbers"][3].asInt());
//
//actor->back_0 = actor->createAnimate(anim["back"]["indexes"][0].asString(), anim["back"]["numbers"][0].asInt());
//actor->back_1 = actor->createAnimate(anim["back"]["indexes"][1].asString(), anim["back"]["numbers"][1].asInt());
//actor->back_2 = actor->createAnimate(anim["back"]["indexes"][2].asString(), anim["back"]["numbers"][2].asInt());
//actor->back_3 = actor->createAnimate(anim["back"]["indexes"][3].asString(), anim["back"]["numbers"][3].asInt());
//
//actor->suffer_0 = actor->createAnimate(anim["suffer"]["indexes"][0].asString(), anim["suffer"]["numbers"][0].asInt());
//actor->suffer_1 = actor->createAnimate(anim["suffer"]["indexes"][1].asString(), anim["suffer"]["numbers"][1].asInt());
//actor->suffer_2 = actor->createAnimate(anim["suffer"]["indexes"][2].asString(), anim["suffer"]["numbers"][2].asInt());
//actor->suffer_3 = actor->createAnimate(anim["suffer"]["indexes"][3].asString(), anim["suffer"]["numbers"][3].asInt());
//
//actor->defend_0 = actor->createAnimate(anim["defend"]["indexes"][0].asString(), anim["defend"]["numbers"][0].asInt());
//actor->defend_1 = actor->createAnimate(anim["defend"]["indexes"][1].asString(), anim["defend"]["numbers"][1].asInt());
//actor->defend_2 = actor->createAnimate(anim["defend"]["indexes"][2].asString(), anim["defend"]["numbers"][2].asInt());
//actor->defend_3 = actor->createAnimate(anim["defend"]["indexes"][3].asString(), anim["defend"]["numbers"][3].asInt());
//
//actor->dead_0 = actor->createAnimate(anim["dead"]["indexes"][0].asString(), anim["dead"]["numbers"][0].asInt());
//actor->dead_1 = actor->createAnimate(anim["dead"]["indexes"][1].asString(), anim["dead"]["numbers"][1].asInt());
//actor->dead_2 = actor->createAnimate(anim["dead"]["indexes"][2].asString(), anim["dead"]["numbers"][2].asInt());
//actor->dead_3 = actor->createAnimate(anim["dead"]["indexes"][3].asString(), anim["dead"]["numbers"][3].asInt());
//
//actor->idle_0 = actor->createAnimate(anim["idle"]["indexes"][0].asString(), anim["idle"]["numbers"][0].asInt());
//actor->idle_1 = actor->createAnimate(anim["idle"]["indexes"][1].asString(), anim["idle"]["numbers"][1].asInt());
//actor->idle_2 = actor->createAnimate(anim["idle"]["indexes"][2].asString(), anim["idle"]["numbers"][2].asInt());
//actor->idle_3 = actor->createAnimate(anim["idle"]["indexes"][3].asString(), anim["idle"]["numbers"][3].asInt());
//
//
//Json::Value attacks = anim["attacks"];
//int size = attacks.size();
//std::vector<cocos2d::Animate*> attack_list;
//attack_list.reserve(size * 4);
//for (int i = 0; i<size; ++i)
//{
//	attack_list.push_back(actor->createAnimate(attacks[i]["indexes"][0].asString(), attacks[i]["numbers"][0].asInt()));
//	attack_list.push_back(actor->createAnimate(attacks[i]["indexes"][1].asString(), attacks[i]["numbers"][1].asInt()));
//	attack_list.push_back(actor->createAnimate(attacks[i]["indexes"][2].asString(), attacks[i]["numbers"][2].asInt()));
//	attack_list.push_back(actor->createAnimate(attacks[i]["indexes"][3].asString(), attacks[i]["numbers"][3].asInt()));
//}
//actor->attackList = attack_list;