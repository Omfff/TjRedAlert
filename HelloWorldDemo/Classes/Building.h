#pragma once
#include"cocos2d.h"
#include"Unit.h"


class Base :public BuildingUnit
{
public:
	//CREATE_FUNC(Base);
	static Base* Base::create(const std::string& filename);
	bool init( CampTypes camp, UnitTypes buildingType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id=0);
	//bool getDamage(int hurt);
};

class PowerPlant :public BuildingUnit
{
public:
	//void update(float f);
	static PowerPlant*PowerPlant::create(const std::string& filename);
	//bool init(CampTypes camp, UnitTypes buildingType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id = 0);
	//bool getDamage(int hurt);//建筑物的getdamage函数可以用函数模板来写

};

class OreRefinery :public BuildingUnit
{
public:
	static OreRefinery * OreRefinery::create(const std::string& filename);
	//bool getDamage(int hurt);//建筑物的getdamage函数可以用函数模板来写
};


class Barracks :public BuildingUnit
{
public:
	GridVec2 findEmptyPosToProduceSolider();
	static Barracks * Barracks::create(const std::string & filename);
};

class WarFactory :public BuildingUnit
{
public:
	GridVec2 findEmptyPosToProduceTank();
	static WarFactory * WarFactory::create(const std::string &filename);
};