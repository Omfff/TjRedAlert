#include"Building.h"

USING_NS_CC;
bool Base::getDamage(int hurt)
{
	if(!Unit::getDamage(hurt))
	{
		unschedule(schedule_selector(Base::update));
		Unit::removeFromMap();
		_tiledMap->removeChild(this);
		return false;
	}
	return true;
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
bool Base::init( CampTypes camp, UnitTypes buildingType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id)
{
	BuildingUnit::init( camp, buildingType, point, map, gridmap,id);
	schedule(schedule_selector(Base::update), 0.1f, kRepeatForever, 0);
	return true;
}