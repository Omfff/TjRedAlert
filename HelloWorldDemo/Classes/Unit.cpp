#include"cocos2d.h"
#include"Unit.h"
#include<cmath>
//#include"Building.h"
#include"UnitManager.h"
USING_NS_CC;
Unit* Unit::create(const std::string& filename)
{
	Unit *ret = new (std::nothrow) Unit();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
bool Unit::init(CampTypes camp, UnitTypes Type, GridVec2 point, TMXTiledMap* map, GridMap *gridmap,int id)
{
	setUnitType(Type);
	return true;
}
void Unit::move()
{
	float moveTime = sqrt(pow((_destination.x / 32.0 - _unitCoord._x), 2) +
		pow(_destination.y / 32.0 - _unitCoord._y, 2)) / 8;//_moveSpeed;
	auto actionMove = MoveTo::create(moveTime, _destination);
	this->runAction(actionMove);
	_battleMap->unitLeavePosition(_unitRect);
	setUnitCoord(GridVec2(float(_destination.x / 32.0), float(_destination.y / 32.0)));
	_unitRect._oriPoint = _unitCoord;
	_battleMap->unitCoordStore(_id, _unitRect);
}
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
bool BuildingUnit::init( CampTypes camp,UnitTypes buildingType,GridVec2 point, TMXTiledMap* map,GridMap * gridmap,int id)
{
	setID(id);
	setCamp(camp);
	setUnitType(buildingType);
	setUnderAttack(false);
	setHealth(HEALTH[buildingType]);
	setMoneyCost(COST[buildingType]);
	setCurrentHp(HEALTH[buildingType]);
	setPowerCost(POWER[buildingType]);
	setMoneyProduce(MONEY_PRODUCE[buildingType]);
	setUnitSize(SIZES[buildingType]);
	setProducingState(0);
	_battleMap = gridmap;
	_tiledMap = map;
	int x = point._x / 32.0;//_tiledMap->getTileSize().width;
	int y = point._y / 32.0;//((_tiledMap->getMapSize().height * _tiledMap->getTileSize().height) - point._y) / _tiledMap->getTileSize().height;
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
void Unit::initHpBar(UnitTypes type)
{
	hpBGSprite = Sprite::create(BUILDING_BG_BAR[type]);
	hpBGSprite->setPosition(Point(this->getContentSize().width / 2,
		this->getContentSize().height));
	hpBGSprite->setScaleX(this->getContentSize().width/hpBGSprite->getContentSize().width );
	hpBGSprite->setScaleY(1.5);
	this->addChild(hpBGSprite);

	_hpBar = LoadingBar::create(BUILDING_HP_BAR[type]);
	//_hpBar->setScaleX(this->getContentSize().width / hpBGSprite->getContentSize().width);
	_hpBar->setDirection(LoadingBar::Direction::LEFT);
	_hpBar->setPercent(100);
	_hpBar->setPosition(Point(_hpBar->getContentSize().width/2,
		_hpBar->getContentSize().height/2));
	hpBGSprite->addChild(_hpBar);
	hideHpBar();
}
void Unit::hideHpBar()
{

	hpBGSprite->setVisible(false);
}
void Unit::displayHpBar()
{
	hpBGSprite->setVisible(true);
}
void BuildingUnit::deleteUnit()
{
	//_tiledMap->removeChild(this);
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
	_battleMap->unitCoordRemove(_id,_unitRect);
	//_tiledMap->removeChild(this, 1);
}
bool Unit::getDamage(int hurt)
{
	setCurrentHp(_currentHp - hurt);
	if (_currentHp <= 0)
	{
		_hpBar->setPercent(0);
		//removeFromMap();
		return false;
	}
	else
	{
		float percent = (float)_currentHp / (float)_health;
		_hpBar->setPercent(percent*100);
		return true;
	}
}
bool FightUnit::init(CampTypes camp, UnitTypes types, GridVec2 coord,
	TMXTiledMap* map, GridMap * gridmap, int id)
{
	int type = types;
	type -= 5;
	setID(id);
	setCamp(camp);
	setHealth(FIGHTER_HEALTH[type]);
	setCurrentHp(_health);
	setUnderAttack(false);
	setUnitSize(FIGHTER_SIZES[type]);
	_battleMap = gridmap;
	_tiledMap = map;
	int x = coord._x / 32.0;
	int y = coord._y / 32.0;
	setUnitCoord(GridVec2(x, y));
	setUnitRect(GridRect(_unitCoord, _unitSize));
	setUnitType(types);
	setAttacking(false);
	setMoveSpeed(UNIT_MOVE_SPEED[type]);
	setAttackSpeed(ATTACK_SPEED[type]);
	setAttackForce(ATTACK_FORCE[type]);
	setAttackID(0);
	setAutoAttack(true);
	setEnermyId(0);
	//setAtkIDPosition()
	setManualAttackScope(MANUAL_ATTACK_RANGE[type]);
	setAutoAttackScope(AUTO_ATTACK_RANGE[type]);
	setPositionInGridMap(_unitRect, gridmap);
	initHpBar(UnitTypes(type));
	//_tiledMap->addChild(this, 1);
	return true;
}
bool FightUnit::setPositionInGridMap(GridRect rectPos, GridMap * map)
{
	if (map->unitCoordStore(_id, rectPos))
		return true;
	else
		return false;
}
bool FightUnit::setPositionInGirdMap(GridRect rectPos, int id)
{
	if (_battleMap->unitCoordStore(_id, rectPos))
		return true;
	else
		return false;
}
bool FightUnit::searchNearEnemy()
{
	auto  autoAtkRect = GridRect(GridVec2(_unitCoord._x - _autoAttackScope._width/ 2, _unitCoord._y - _autoAttackScope._height/ 2 ),
		_autoAttackScope);
	auto  unitIDs = _battleMap->getUnitIdAt(autoAtkRect);
	int distance = 0;
	int minDistance = 10000;
	int mark = 0;
	if(unitIDs.empty()!=true)
		for (auto itsID : unitIDs)
		{
			int camp=_unitManager->getUnitCamp(itsID);
			if (camp!= 0 && camp != _camp)//&&itsID!=_id )//
			{
				GridVec2 enermyPos=_unitManager->getUnitPos(itsID);
				distance = (enermyPos._x - _unitCoord._x)*(enermyPos._x - _unitCoord._x) +
							(enermyPos._y - _unitCoord._y)*(enermyPos._y - _unitCoord._y);
				if (distance < minDistance)
				{
					_attackID = itsID;
					mark = 1;
					//_autoAttack = true;
				}
			}
		}
	if (mark == 1)
		return true;
	else
		return false;
}
void FightUnit::attack()
{
	shootBullet();
	
}
/*void FightUnit::autoAttack()
{

}*/
void FightUnit::move()
{
	//根据寻路算法找到路径
	//产生移动的消息
	
	float moveTime = sqrt(pow((_destination.x/32.0 - _unitCoord._x), 2) +
		pow(_destination.y/32.0 - _unitCoord._y, 2))/_moveSpeed;
	auto actionMove = MoveTo::create(moveTime, _destination);
	this->runAction(actionMove);
	_battleMap->unitLeavePosition(_unitRect);
	setUnitCoord(GridVec2(float(_destination.x/32.0),float(_destination.y/32.0)));
	_unitRect._oriPoint = _unitCoord;
	_battleMap->unitCoordStore(_id,_unitRect);
	
}
void BuildingUnit::startProduceUnit(UnitTypes proUnitType)
{
	setProducingState(1);
	_produceProcess = 0;
	_producingUnitType = proUnitType;
	if (_producingUnitType == GI)
		_produceTime = PRODUCEGITIME;
	else if (_producingUnitType == ATTACKDOG)
		_produceTime = PRODUCEATTACKDOGTIME;
	else
		_produceTime = PRODUCETANKTIME;
	this->schedule(schedule_selector(BuildingUnit::produceUpdate));
}
void BuildingUnit::stopProduceUnit()
{
	this->unschedule(schedule_selector(BuildingUnit::produceUpdate));
}
void BuildingUnit::produceUpdate(float ft)
{
	if (_producingState == 1)
	{
		if (++_produceProcess >= _produceTime)
		{
			_producingState = 0;
			_produceProcess = 0;
			GridVec2 producePos = findEmptyPosToProduce();
			_unitManager->creatProduceMessage(_producingUnitType, GridVec2(producePos._x / 32, producePos._y / 32));
			_unitManager->creatUnit(_unitManager->getPlayerCamp(), _producingUnitType, producePos);
			stopProduceUnit();
		}
	}
}
void FightUnit::shootBullet()
{
}
