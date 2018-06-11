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
bool Unit::init(CampTypes camp, UnitTypes Type, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id)
{
	setUnitType(Type);
	return true;
}

void Unit::move()
{
	/*float moveTime = sqrt(pow((_destination.x / 32.0 - _unitCoord._x), 2) +
		pow(_destination.y / 32.0 - _unitCoord._y, 2)) / 8;//_moveSpeed;
	auto actionMove = MoveTo::create(moveTime, _destination);
	this->runAction(actionMove);
	_battleMap->unitLeavePosition(_unitRect);
	setUnitCoord(GridVec2(float(_destination.x / 32.0), float(_destination.y / 32.0)));
	_unitRect._oriPoint = _unitCoord;
	_battleMap->unitCoordStore(_id, _unitRect);*/
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
	//setAtkIDPosition()
	setManualAttackScope(MANUAL_ATTACK_RANGE[type]);
	setAutoAttackScope(AUTO_ATTACK_RANGE[type]);

	setPositionInGirdMap(_unitRect, id);
	initHpBar(UnitTypes(type));
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

void FightUnit::tryToFindPath()
{
	if (_camp != _unitManager->getPlayerCamp())
	{
		return;
	}
	GridVec2 destination = GridVec2(_destination.x / 32, _destination.y / 32);
	if (_battleMap->checkPointPosition(destination)) {
		destination = _battleMap->getEmptyPointNearby(destination);
		_destination.x = destination._x * 32 + 16;
		_destination.y = destination._y * 32 + 16;
	}
	_gridPath = findPath(destination);
	optimizePath();

}

std::vector<GridVec2> FightUnit::findPath(const GridVec2 & destination)
{
	std::vector<std::vector<int>> & gridMap = _battleMap->_findPathMap;
	GridVec2 start = getUnitCoord();

	PathFinder pathFinder(gridMap, start._x, start._y, destination._x, destination._y);
	pathFinder.searchPath();
	pathFinder.generatePath();
	std::vector<GridVec2> gridPath = pathFinder.getPath();
	return gridPath;
}

void FightUnit::optimizePath()
{
	int pathLength = _gridPath.size();
	if (pathLength < 3) {
		return;
	}

	std::vector<GridVec2> optimizingPath;
	GridVec2 previousPosition = _gridPath[0];
	GridVec2 previousDirection(2, 3);

	for (int i = 1; i < pathLength - 1; ++i) {
		const auto & p = _gridPath[i];
		GridVec2 dir;

		if ((p - previousPosition)._x < 0) {
			dir._x = -1;
		}
		else if ((p - previousPosition)._x == 0) {
			dir._x = 0;
		}
		else {
			dir._x = 1;
		}

		if ((p - previousPosition)._y < 0) {
			dir._y = -1;
		}
		else if ((p - previousPosition)._y == 0) {
			dir._y = 0;
		}
		else {
			dir._y = 1;
		}

		if (!(dir == previousDirection)) {
			optimizingPath.push_back(previousPosition);
			previousDirection = dir;
		}
		previousPosition = p;
	}

	optimizingPath.push_back(_gridPath[pathLength - 1]);
	_gridPath.clear();
	for (auto grid : optimizingPath) {
		_gridPath.push_back(grid);
	}
}

void FightUnit::searchNearEnemy()
{
	const auto & autoAtkRect = GridRect(GridVec2((_unitCoord._x - _autoAttackScope._width )/ 2, (_unitCoord._y - _autoAttackScope._height) / 2 ),
		_autoAttackScope);
	const auto & unitIDs = _battleMap->getUnitIdAt(autoAtkRect);
	for (auto itsID : unitIDs)
	{
		int camp=_unitManager->getUnitCamp(itsID);
		if (camp!= 0 && camp != _camp)
		{
			_attackID = itsID;
			_autoAttack = true;
			return;
		}
	}
	return;
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
	/*GridVec2 destination(_destination.x / 32, _destination.y / 32);
	if (_battleMap->checkPointPosition(destination)) {
		destination = _battleMap->getEmptyPointNearby(destination);
		_destination = GridVec2(destination._x, destination._y);
	}*/
	//产生移动的消息


	if (_gridPath.size() == 1) {
		float distance = sqrt(pow(this->getUnitCoord()._x - _gridPath[0]._x, 2) + pow(this->getUnitCoord()._y - _gridPath[0]._y, 2));
		float moveTime = distance / (UNIT_MOVE_SPEED[this->getUnitType() - 5] * 3);
		Vec2 endPosition(_gridPath[0]._x * 32 + 16, _gridPath[0]._y * 32 + 16);

		this->runAction(MoveTo::create(moveTime, endPosition));
		_battleMap->unitLeavePosition(this->getUnitRect());
		setUnitCoord(_gridPath[0]);
		_unitRect._oriPoint = _unitCoord;
		_battleMap->unitCoordStore(_id, _unitRect);

	}
	else {
		Vector<FiniteTimeAction*> actionSequence;

		for (vector<GridVec2>::reverse_iterator it = _gridPath.rbegin(); it != _gridPath.rend() - 1; ++it) {
			GridVec2 start(*it);
			GridVec2 end(*(it + 1));
			float distance = sqrt(pow(start._x - end._x, 2) + pow(start._y - end._y, 2));
			float moveTime = distance / (UNIT_MOVE_SPEED[this->getUnitType() - 5] * 3);
			Vec2 endPosition(end._x * 32 + 16, end._y * 32 + 16);

			actionSequence.pushBack(MoveTo::create(moveTime, endPosition));
			_battleMap->unitLeavePosition(this->getUnitRect());
			setUnitCoord(end);
			_unitRect._oriPoint = _unitCoord;
			_battleMap->unitCoordStore(_id, _unitRect);
		}

		auto moveSequence = Sequence::create(actionSequence);
		this->runAction(moveSequence);

	}
}
void FightUnit::shootBullet()
{
}