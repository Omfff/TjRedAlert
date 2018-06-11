#pragma once
#include"UnitManager.h"
#include"cocos2d.h"
#include"Unit.h"
#include"Fighter.h"
#include"Building.h"


#define _GI_BULLET_SPEED 10;
#define _TANK_BULLET_SPEED 20;
USING_NS_CC;
class Unit;
class Bullet :public Sprite
{
public:
	static Bullet* create(const std::string& filename);
	bool initWithTarget(FightUnit* sender, int target);
	void setMap(TMXTiledMap *map);
	void setUnitManager(UnitManager * uM);
	//~Bullet();
private:
	TMXTiledMap * _tiledMap = nullptr;
	Sprite * _bullet;
	Unit* _target=nullptr;
	void update(float dt);
	int _speed;
	int _targetId;
	FightUnit * _sender;
	UnitManager * _unitManager;


};

