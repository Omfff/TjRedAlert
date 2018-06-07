#include"Buliding.h"

USING_NS_CC;
/*bool Base::getDamage(int hurt)
{
	if(!Unit::getDamage(hurt))
	{
		Unit::removeFromMap();
		//_tiledMap->removeChild(this);
		return false;
	}
	return true;
}
*/

/*void Base::update(float f)
{
	getDamage(100);
}*/
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
	//schedule(schedule_selector(Base::update), 0.1f, kRepeatForever, 0);
	return true;
}
PowerPlant* PowerPlant::create(const std::string& filename)
{
	PowerPlant *ret = new (std::nothrow) PowerPlant();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
OreRefinery * OreRefinery::create(const std::string& filename)
{
	OreRefinery  *ret = new (std::nothrow) OreRefinery();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
Barracks *  Barracks::create(const std::string& filename)
{
	Barracks  *ret = new (std::nothrow)  Barracks();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
WarFactory *  WarFactory::create(const std::string& filename)
{
	WarFactory  *ret = new (std::nothrow)  WarFactory();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
GridVec2 Barracks::findEmptyPosToProduceSolider()
{
	GridVec2 tempPos = _battleMap->getEmptyPointNearby(_unitCoord);
	return GridVec2(tempPos._x * 32, tempPos._y * 32);
}
GridVec2 WarFactory::findEmptyPosToProduceTank()
{
	GridVec2 tempPos = _battleMap->getEmptyPointNearby(_unitCoord);
	return GridVec2(tempPos._x * 32, tempPos._y * 32);
}