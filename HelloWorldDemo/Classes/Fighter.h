#pragma once
#include"Unit.h"
#include"cocos2d.h"
#include "Bullet.h"


class Bullet;
class Solider :public FightUnit
{
public:
	static Solider *Solider::create(const std::string& filename);
	//bool init(CampTypes camp, UnitTypes fightType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id = 0);
	void shootBullet();
	void attack();
	void attackUpdate(float dt);
	void startAttackUpdate();
	void stopAttackUpdate();
};
class Tank :public FightUnit
{
public:
	static Tank*Tank::create(const std::string& filename);
	//bool init(CampTypes camp, UnitTypes fightType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id = 0);
	void shootBullet();
	void attack();
	void attackUpdate(float dt);
	void startAttackUpdate();
	void stopAttackUpdate();
};
