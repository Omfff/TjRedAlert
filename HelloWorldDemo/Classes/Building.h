#pragma once
#include"cocos2d.h"
#include"Unit.h"

class Base :public BuildingUnit
{
public:
	void startProduce();
	//CREATE_FUNC(Base);
	void update(float f);
	static Base* Base::create(const std::string& filename);
	bool init(int id, CampTypes camp, BuildingTypes buildingType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap);
	void getDamage(int hurt);
};
