#pragma once
#include "cocos2d.h"
#include <json/json.h>

typedef std::function<void()> AttackCallBack;

class Actor : public cocos2d::Sprite
{
	enum CombatStatus {
		UNPREPARED = 0,
		PREPARED = 1,
		ON_ATTACK = 2,
	};
public:
	int id;
	std::string name;
	int pos_combat;
	std::string path;
	Json::Value anim;

	static Actor* createActor(int index);
	virtual bool init();
	CREATE_FUNC(Actor);

	void attack(Actor* target, int type, const AttackCallBack& callback);

	void suffer(float time);

	void defend();

	void idle();

	void dead();

	void stand(int type);

	void walk(cocos2d::Vec2 target, bool canMove);

	void setSpeed(float speed) { _speed = speed; }
	float getSpeed() { return _speed; }

	void setStatus(CombatStatus status) { _status = status; }
	CombatStatus getStatus() { return _status; }


private:
	cocos2d::Animate* createAnimate(const std::vector<std::string>& names, float delay);

	cocos2d::Animate* createAnimate(const std::string& prefix, const int& count, float delay = 0.08f, std::string type = ".tga", bool reverse = false);
	//meta
	//animations

	float _speed;
	CombatStatus _status;//0 未准备 1已准备 2施法中 

};