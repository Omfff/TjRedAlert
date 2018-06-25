#include"cocos2d.h"
#include"Unit.h"
#include<cmath>
//#include"Building.h"
#include"UnitManager.h"
USING_NS_CC;

const string CHOOSE_RESOURCES[8][5] =
{ { "", "units/Base(red).png", "units/Base(blue).png", "units/Base(green).png", "units/Base(yellow).png" },
{ "", "units/PowerPlant(red).png", "units/PowerPlant(blue).png", "units/PowerPlant(green).png", "units/PowerPlant(yellow).png" },
{ "", "units/Barrack(red).png", "units/Barrack(blue).png", "units/Barrack(green).png", "units/Barrack(yellow).png" },
{ "", "units/WarFactory(red).png", "units/WarFactory(blue).png", "units/WarFactory(green).png", "units/WarFactory(yellow).png" },
{ "", "units/OreRefinery(red).png", "units/OreRefinery(blue).png", "units/OreRefinery(green).png", "units/OreRefinery(yellow).png" },
{ "", "units/GI(red).png", "units/GI(blue).png", "units/GI(green).png", "units/GI(yellow).png" },
{ "", "units/AttackDog(red).png", "units/AttackDog(blue).png", "units/AttackDog(green).png", "units/AttackDog(yellow).png" },
{ "", "units/Tank(red).png", "units/Tank(blue).png", "units/Tank(green).png", "units/Tank(yellow).png" } };

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


	/*animationCache->addAnimation(baseAnimation, "BASE");
	animationCache->addAnimation(powerPlantAnimation, "POWERPLANT");
	animationCache->addAnimation(barrackAnimation, "BARRACK");
	animationCache->addAnimation(warFactoryAnimation, "WARFACTORY");
	animationCache->addAnimation(oreRefineryAnimation, "OREREFINERY");*/

	return true;
}

void Unit::move()
{

}

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
void BuildingUnit::buildingUpdate()
{
	_progressbgSprite = Sprite::create("slider grey.png");
	_progressbgSprite->setPosition(Point(this->getContentSize().width / 2,
		this->getContentSize().height));
	_progressbgSprite->setScaleX(this->getContentSize().width / _progressbgSprite->getContentSize().width);
	_progressbgSprite->setScaleY(1.5);
	this->addChild(_progressbgSprite);

	auto progress = LoadingBar::create("slider green.png");
	progress->setDirection(LoadingBar::Direction::LEFT);
	progress->setPosition(Point(progress->getContentSize().width / 2,
		progress->getContentSize().height / 2));//位置
	progress->setTag(100);
	progress->setPercent(0);
	_progressbgSprite->addChild(progress);
	setIsBuilding(true);
	setOpacity(100);
	this->schedule(schedule_selector(BuildingUnit::updateBuildingBar));
}
void BuildingUnit::updateBuildingBar(float dt) 
{
	_buildingProgress= _buildingProgress + BUILDINGTIME[_unitType];
	if (_buildingProgress <= 100)
	{
		auto progress = static_cast<LoadingBar*>(_progressbgSprite->getChildByTag(100));
		progress->setPercent(_buildingProgress);
	}
	else 
	{
		this->removeChild(_progressbgSprite,true);
		setIsBuilding(false);
		this->unschedule(schedule_selector(BuildingUnit::updateBuildingBar));
		if(_camp==_unitManager->getPlayerCamp())
			SimpleAudioEngine::getInstance()->playEffect("Music/Construction complete.wav");//音效
		setOpacity(255);
		//AnimationCache * animationCache = AnimationCache::sharedAnimationCache();
		auto animation = Animation::create();
		switch (this->getUnitType())
		{
		case BASE:
			animation = AnimationCache::getInstance()->getAnimation("BASE");
			//animation = animationCache->animationByName("BASE");
			//Animation * animation = animationCache->animationByName("BASE");
			break;
		case POWERPLANT:
			animation = AnimationCache::getInstance()->getAnimation("POWERPLANT");
			break;
		case BARRACKS:
			animation = AnimationCache::getInstance()->getAnimation("BARRACK");
			break;
		case WARFACTORY:
			animation = AnimationCache::getInstance()->getAnimation("WARFACTORY");
			break;
		case OREREFINERY:
			animation = AnimationCache::getInstance()->getAnimation("OREREFINERY");
			break;
		default:
			break;
		}
		auto action = Animate::create(animation);
		auto sequence = Sequence::create(action, CallFunc::create([&]() {this->setTexture(CHOOSE_RESOURCES[this->getUnitType()][this->getCamp()]); }), NULL);
		this->runAction(sequence);

		if(_camp==_unitManager->getPlayerCamp())
		{
			_unitManager->getmoney()->addMoneyInPeriod(MONEY_PRODUCE[_unitType]);
			if (POWER[_unitType] > 0)
				_unitManager->getelectricity()->addElectricity(POWER[_unitType]);
			else
				_unitManager->getelectricity()->costElectricity(-POWER[_unitType]);
		}
			
	}
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
	if (_camp == _unitManager->getPlayerCamp())
	{
		if (_unitType < 5)
			SimpleAudioEngine::getInstance()->playEffect("Music/Explosion.wav");//音效
		//unschedule(schedule_selector(update));
		else
			SimpleAudioEngine::getInstance()->playEffect("Music/Solider explosion.wav");
	}
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
		_hpBar->setPercent(percent * 100);
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

void FightUnit::tryToFindPath()
{
	if (_camp != _unitManager->getPlayerCamp())
	{
		return;
	}
	GridVec2 destination = GridVec2(_destination.x / 32, _destination.y / 32);
	if (_battleMap->checkPointPosition(destination)) 
	{
		destination = _battleMap->getEmptyPointNearby(destination);
	}
	if (_unitManager->_destinationMap.count(destination)) 
	{
		destination = _battleMap->getNewDestination(destination,_unitManager->_destinationMap);
	}
	_destination.x = destination._x * 32 + 16;
	_destination.y = destination._y * 32 + 16;
	_unitManager->_destinationMap.insert(pair<GridVec2, int>(destination, getID()));
	_gridPath = findPath(destination);
	optimizePath();
	_unitManager->creatMoveMessage(_id, _gridPath);
}

std::vector<GridVec2> FightUnit::findPath(const GridVec2 & destination)
{
	std::vector<std::vector<int>> gridMap = _battleMap->_findPathMap;
	GridVec2 start = getUnitCoord();

	PathFinder pathFinder(gridMap, start._x, start._y, destination._x, destination._y);
	pathFinder.searchPath();
	pathFinder.generatePath();
	std::vector<GridVec2> gridPath = pathFinder.getPath();
	if (gridPath.size() == 0) 
	{
		for (auto unit : _unitManager->_unitIdMap) 
		{
			if (unit.second->getUnitType() >= 5) 
			{
				gridMap[unit.second->getUnitCoord()._x][unit.second->getUnitCoord()._y] = 0;
				//pathFinder._mapGrid[_unitManager->getUnitPos(unitid)._x][_unitManager->getUnitPos(unitid)._y].setStatus(UNOCCUPIED);
			}
		}
		PathFinder pathFinderAgain(gridMap, start._x, start._y, destination._x, destination._y);
		pathFinderAgain.searchPath();
		pathFinderAgain.generatePath();
		gridPath = pathFinderAgain.getPath();
	}
	return gridPath;
}

void FightUnit::optimizePath()
{
	int pathLength = _gridPath.size();
	if (pathLength < 3) 
	{
		return;
	}

	std::vector<GridVec2> optimizingPath;
	GridVec2 previousPosition = _gridPath[0];
	GridVec2 previousDirection(2, 3);

	for (int i = 1; i < pathLength - 1; ++i) 
	{
		const auto & p = _gridPath[i];
		GridVec2 dir;

		if ((p - previousPosition)._x < 0) 
		{
			dir._x = -1;
		}
		else if ((p - previousPosition)._x == 0) 
		{
			dir._x = 0;
		}
		else 
		{
			dir._x = 1;
		}

		if ((p - previousPosition)._y < 0) 
		{
			dir._y = -1;
		}
		else if ((p - previousPosition)._y == 0)
		{
			dir._y = 0;
		}
		else
		{
			dir._y = 1;
		}

		if (!(dir == previousDirection)) 
		{
			optimizingPath.push_back(previousPosition);
			previousDirection = dir;
		}
		previousPosition = p;
	}

	optimizingPath.push_back(_gridPath[pathLength - 1]);
	_gridPath.clear();
	for (auto grid : optimizingPath) 
	{
		_gridPath.push_back(grid);
	}
}
int FightUnit::searchNearEnemy()
{
	GridVec2 autoAtkRectPoint = GridVec2((_unitCoord._x - _autoAttackScope._width / 2 )< 0 ? 0 : (_unitCoord._x - _autoAttackScope._width / 2),
								(_unitCoord._y - _autoAttackScope._height / 2) < 0? 0 : (_unitCoord._y - _autoAttackScope._height / 2));
	GridDimen attackSize;
	if (_unitCoord._x + _autoAttackScope._width / 2 >= _MAP_WIDTH)
		attackSize._width = _autoAttackScope._width - ((_unitCoord._x + _autoAttackScope._width / 2) - _MAP_WIDTH + 1);
	else
		attackSize._width = _autoAttackScope._width;
	if (_unitCoord._y + _autoAttackScope._height / 2 >= _MAP_HEIGHT)
		attackSize._height = _autoAttackScope._height - ((_unitCoord._y + _autoAttackScope._height / 2) - _MAP_HEIGHT + 1);
	else
		attackSize._height = _autoAttackScope._height;
	auto  autoAtkRect = GridRect(autoAtkRectPoint,attackSize);
	int attackId;
	auto  unitIDs = _battleMap->getUnitIdAt(autoAtkRect);
	int distance = 0;
	int minDistance = 10000;
	int mark = 0;
	if(unitIDs.empty()!=true)
		for (auto itsID : unitIDs)
		{
			int camp=_unitManager->getUnitCamp(itsID);
			if (camp!= 0 && camp != _camp&&
				_unitManager->_unitIdMap.count(itsID)>0&&
				_unitManager->_unitIdMap[itsID]->getActionManager()->getNumberOfRunningActionsInTarget(_unitManager->_unitIdMap[itsID]) == 0&&
				_unitManager->_unitIdMap[itsID]->getIsBuilding()==false)////itsID != _id)
			{
				if (_unitType == ATTACKDOG && (_unitManager->_unitIdMap[itsID]->getUnitType() != ATTACKDOG
					&& _unitManager->_unitIdMap[itsID]->getUnitType() != GI))
					continue;
				GridVec2 enermyPos=_unitManager->getUnitPos(itsID);
				distance = (enermyPos._x - _unitCoord._x)*(enermyPos._x - _unitCoord._x) +
							(enermyPos._y - _unitCoord._y)*(enermyPos._y - _unitCoord._y);
				if (distance < minDistance)
				{
					attackId = itsID;
					mark = 1;
					//_autoAttack = true;
				}
			}
		}
	if (mark == 1)
		return attackId;
	else
		return 0;
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
	setAutoAttack(false);
	if (_gridPath.size() == 1) 
	{
		float distance = sqrt(pow(this->getUnitCoord()._x - _gridPath[0]._x, 2) + pow(this->getUnitCoord()._y - _gridPath[0]._y, 2));
		float moveTime = distance / (UNIT_MOVE_SPEED[this->getUnitType() - 5]);
		Vec2 endPosition(_gridPath[0]._x * 32 + 16, _gridPath[0]._y * 32 + 16);

		this->runAction(MoveTo::create(moveTime, endPosition));
		_battleMap->unitLeavePosition(this->getUnitRect());
		setUnitCoord(_gridPath[0]);
		_unitRect._oriPoint = _unitCoord;
		_battleMap->unitCoordStore(_id, _unitRect);

	}
	else 
	{
		Vector<FiniteTimeAction*> actionSequence;

		for (vector<GridVec2>::reverse_iterator it = _gridPath.rbegin(); it != _gridPath.rend() - 1; ++it) 
		{
			GridVec2 start(*it);
			GridVec2 end(*(it + 1));
			float distance = sqrt(pow(start._x - end._x, 2) + pow(start._y - end._y, 2));
			float moveTime = distance / (UNIT_MOVE_SPEED[this->getUnitType() - 5]);
			Vec2 endPosition(end._x * 32 + 16, end._y * 32 + 16);

			actionSequence.pushBack(MoveTo::create(moveTime, endPosition));
		}

		_battleMap->unitLeavePosition(this->getUnitRect());
		setUnitCoord(_gridPath[0]);
		_unitRect._oriPoint = _unitCoord;
		_battleMap->unitCoordStore(_id, _unitRect);

		auto moveSequence = Sequence::create(actionSequence);
		auto sequence = Sequence::create(moveSequence, CallFunc::create(CC_CALLBACK_0(FightUnit::moveEndedCallBack, this)), NULL);
		if(_camp==_unitManager->getPlayerCamp())
			SimpleAudioEngine::getInstance()->playEffect("Music/On my way.wav");//音效
		this->runAction(sequence);
	}

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
			_unitManager->creatProduceMessage(_producingUnitType, GridVec2(producePos._x +16, producePos._y+16 ));
			if (_producingUnitType== GI)
				_unitManager->setWaitingGINum(_unitManager->getWaitingGINum()-1);
			else if(_producingUnitType ==ATTACKDOG)
				_unitManager->setWaitingAttackDogNum(_unitManager->getWaitingAttackDogNum() - 1);
			else
				_unitManager->setWaitingTankNum(_unitManager->getWaitingTankNum() - 1);
			//_unitManager->creatUnit(_unitManager->getPlayerCamp(), _producingUnitType, producePos);
			stopProduceUnit();
		}
	}
}
void FightUnit::shootBullet()
{
	
}
void FightUnit::moveEndedCallBack() 
{
	_unitManager->_destinationMap.erase(GridVec2(_destination.x / 32, _destination.y / 32));
	if (_attackID != 0&&_camp==_unitManager->getPlayerCamp())
		this->startAttackUpdate();
	this->setAutoAttack(true);
}

