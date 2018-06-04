#pragma once
#include"cocos2d.h"
#include"Unit.h"

//class UnitManager;
class Base :public BuildingUnit
{
public:
	void startProduce();
	//CREATE_FUNC(Base);
	void update(float f);
	static Base* Base::create(const std::string& filename);
	bool init( CampTypes camp, UnitTypes buildingType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id=0);
	bool getDamage(int hurt);
};
class PowerPlant :public BuildingUnit
{
public:
	void startProduce();
	void update(float f);
	static PowerPlant*PowerPlant::create(const std::string& filename);
	bool init(CampTypes camp, UnitTypes buildingType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id = 0);
	bool getDamage(int hurt);//建筑物的getdamage函数可以用函数模板来写

};
