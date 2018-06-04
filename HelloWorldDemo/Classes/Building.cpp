#include"Building.h"
USING_NS_CC;
void Base::getDamage(int hurt)
{
	Unit::getDamage(hurt);
	if (_currentHp <= 0)
	{
		Unit::removeFromMap();
		_tiledMap->removeChild(this);
	}
}
void Base::startProduce()
{
log("money++");
}
void Base::update(float f)
{
	startProduce();
	//getDamage(100);
}
Base* Base::create(const std::string& filename)
{
	Base *ret = new (std::nothrow) Base();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
bool Base::init(int id, CampTypes camp, BuildingTypes buildingType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap)
{
	BuildingUnit::init(id, camp, buildingType, point, map, gridmap);
	schedule(schedule_selector(Base::update), 0.1f, kRepeatForever, 0);
	return true;
}