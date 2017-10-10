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
	AttackCallBack userAction;
	AttackCallBack deadAction;

	int blood;
	static Actor* createActor(int index);
	virtual bool init();
	CREATE_FUNC(Actor);

	void idle();
	void dead();
	void defend();
	void stand(int type);
	void suffer(float time);
	void walk(cocos2d::Vec2 target, bool canMove);
	void attack(Actor* target, int type/*, const AttackCallBack& callback*/);

	inline void setSpeed(float speed) { _speed = speed; }
	inline float getSpeed() { return _speed; }

	inline void setStatus(CombatStatus status) { _status = status; }
	inline CombatStatus getStatus() { return _status; }

	void setBloodBarDisplay(bool show);
	void updateBloodBarValue();
	void showDamageValues();
private:

	//meta

	float _speed;
	CombatStatus _status;//0 未准备 1已准备 2施法中 

};