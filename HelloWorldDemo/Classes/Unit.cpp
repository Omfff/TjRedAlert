#include"cocos2d.h"
#include"Unit.h"
#include"Buliding.h"
USING_NS_CC;

/*Unit* Unit::create(const std::string& filename)
{
	Unit *ret = new (std::nothrow) Unit();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}*/
/*BuildingUnit* BuildingUnit::create(const std::string& filename)
{
	BuildingUnit *ret = new (std::nothrow) BuildingUnit();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}*/
bool BuildingUnit::init(int id, CampTypes camp,BuildingTypes buildingType,GridVec2 point, TMXTiledMap* map,GridMap * gridmap)
{
	if (!Sprite::init())
	{
		return false;
	}
	setID(id);
	setCamp(camp);
	setBuildingType(buildingType);
	setUnderAttack(false);
	setHealth(HEALTH[buildingType]);
	setMoneyCost(COST[buildingType]);
	setCurrentHp(HEALTH[buildingType]);
	setPowerCost(POWER[buildingType]);
	setMoneyProduce(MONEY_PRODUCE[buildingType]);
	setUnitSize(SIZES[buildingType]);
	_battleMap = gridmap;
	_tiledMap = map;
	int x = point._x / _tiledMap->getTileSize().width;
	int y = ((_tiledMap->getMapSize().height * _tiledMap->getTileSize().height) - point._y) / _tiledMap->getTileSize().height;
	setUnitCoord(GridVec2(x,y));
	setUnitRect(GridRect(_unitCoord,_unitSize));
	
	setPositionInGridMap(_unitRect,gridmap);
	//_tiledMap->addChild(this, 1);
	initHpBar(buildingType);
	return true;
}

bool BuildingUnit::setPositionInGridMap(GridRect rectPos, GridMap * map)
{
	if (map->unitCoordStore(_id, rectPos))
		return true;
	else
		return false;
}
void BuildingUnit::initHpBar(BuildingTypes type)
{
	hpBGSprite = Sprite::create(BUILDING_BG_BAR[type]);
	hpBGSprite->setPosition(Point(this->getContentSize().width / 2,
		this->getContentSize().height));
	this->addChild(hpBGSprite);

	_hpBar = LoadingBar::create(BUILDING_HP_BAR[type]);
	_hpBar->setDirection(LoadingBar::Direction::LEFT);
	_hpBar->setPercent(100);
	_hpBar->setPosition(Point(_hpBar->getContentSize().width/2,
		_hpBar->getContentSize().height/2));
	hpBGSprite->addChild(_hpBar);
}
void BuildingUnit::deleteUnit()
{
	_tiledMap->removeChild(this);
}
bool ExplosionEffect::init()
{
	if (!ParticleFire::init())
		return false;
	setScale(0.0000008);
	setDuration(1);
	auto action = ScaleBy::create(0.5, 500000);
	runAction(action);
	scheduleOnce(schedule_selector(ExplosionEffect::remove), 1.2);
	setPositionType(PositionType::RELATIVE);
	return true;
}

void ExplosionEffect::remove(float f)
{
	removeFromParent();
}
void Unit::removeFromMap()
{
	auto explosion_effect = ExplosionEffect::create();
	explosion_effect->setPosition(this->getPosition());
	getParent()->addChild(explosion_effect, 20);
	//unschedule(schedule_selector(update));
	_battleMap->unitCoordRemove(_id);
	//_tiledMap->removeChild(this, 1);
}
void Unit::getDamage(int hurt)
{
	setCurrentHp(_currentHp - hurt);
	if (_currentHp <= 0)
	{
		unschedule(schedule_selector(Base::update));
	}
	else
	{
		float percent = (float)_currentHp / (float)_health;
		_hpBar->setPercent(percent*100);
	}
}
bool FightUnit::init(int id,GridVec2 coord, CampTypes camp, FightUnitType type,
 TMXTiledMap* map, GridMap * gridmap)
{
	if (!Sprite::init())
	{
		return false;
	}
	setID(id);
	setCamp(camp);
	setHealth(FIGHTER_HEALTH[type]);
	setCurrentHp(_health);
	setUnderAttack(false);
	setUnitSize(FIGHTER_SIZES[type]);
	_battleMap = gridmap;
	_tiledMap = map;
	int x = coord._x / _tiledMap->getTileSize().width;
	int y = ((_tiledMap->getMapSize().height * _tiledMap->getTileSize().height) - coord._y) / _tiledMap->getTileSize().height;
	setUnitCoord(GridVec2(x, y));
	setUnitRect(GridRect(_unitCoord, _unitSize));
	setFighterType(type);
	setAttacking(false);
	setMoveSpeed(UNIT_MOVE_SPEED[type]);
	setAttackSpeed(ATTACK_SPEED[type]);
	setAttackForce(ATTACK_FORCE[type]);
	setAttackID(0);
	//setAtkIDPosition()
	setManualAttackScope(MANUAL_ATTACK_RANGE[type]);
	setAutoAttackScope(AUTO_ATTACK_RANGE[type]);

	setPositionInGirdMap(_unitRect,id);
	//_tiledMap->addChild(this, 1);
	return true;
}
bool FightUnit::setPositionInGirdMap(GridRect rectPos, int id)
{
	if (_battleMap->unitCoordStore(_id, rectPos))
		return true;
	else
		return false;
}
void FightUnit::searchNearEnemy()
{
	const auto & autoAtkRect = GridRect(_unitCoord - _autoAttackScope / 2, _autoAttackScope);
	const auto & unitIDs = _battleMap->getUnitIdAt(autoAtkRect);
	for (auto itsID : unitIDs)
	{
		//int its_camp = unit_manager->getUnitCamp(itsID);
		if (its_camp != 0 && its_camp != _camp)
		{
			_attackID = itsID;
			return;
		}
	}
	return;
}
void FightUnit::attack()
{

}
void FightUnit::autoAttack()
{

}
void FightUnit::move()
{

}
*/