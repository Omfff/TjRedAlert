#include"Fighter.h"
Solider* Solider::create(const std::string& filename)
{
	Solider *ret = new (std::nothrow) Solider();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
bool Solider::init(CampTypes camp, UnitTypes types, GridVec2 coord,
	TMXTiledMap* map, GridMap * gridmap, int id)
{
	FightUnit::init(camp, types, coord, map, gridmap,id);
	this->schedule(schedule_selector(Solider::autoAttack));
	return true;
}
bool Tank::init(CampTypes camp, UnitTypes types, GridVec2 coord,
	TMXTiledMap* map, GridMap * gridmap, int id)
{
	FightUnit::init(camp, types, coord, map, gridmap, id);
	this->schedule(schedule_selector(Solider::autoAttack));
	return true;
}
Tank*Tank::create(const std::string& filename)
{
	Tank *ret = new (std::nothrow) Tank();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
void Tank::attack()
{
	shootBullet();
}
void Tank::shootBullet()
{
	Bullet* bullet = Bullet::create("Bullet.png");
	bullet->setUnitManager(_unitManager);
	bullet->setMap(_tiledMap);
	bullet->initWithTarget(this, _attackID);
}
void Tank::attackUpdate(float dt)
{
	if (_unitManager->_unitIdMap.count(_attackID)>0)//if (_unitManager->_unitIdMap[_attackID]!=nullptr)
	{
		_unitManager->createAttackMessage(_id, _attackID, _attackForce);
		/*attack();
		if (_unitManager->_unitIdMap.count(_attackID)>0)//if (_unitManager->_unitIdMap[_attackID]!=nullptr)
		{
			_unitManager->_unitIdMap[_attackID]->getDamage(_attackForce);
			if (_unitManager->_unitIdMap[_attackID]->getCurrentHp() <= 0)
			{
				_unitManager->destoryUnit(_attackID);
				setAttackID(0);
				stopAttackUpdate();
			}
			else
				if (_unitManager->_unitIdMap[_attackID]->getCamp() == _unitManager->getPlayerCamp())
					_unitManager->_unitIdMap[_attackID]->displayHpBar();
		}
		else
			stopAttackUpdate();*/
	}
	else
		stopAttackUpdate();
}
void Tank::startAttackUpdate()
{
	this->schedule(schedule_selector(Tank::attackUpdate), 0.5f);
}
void Tank::stopAttackUpdate()
{
	this->unschedule(schedule_selector(Tank::attackUpdate));
}
void Solider::attack()
{
	shootBullet();
}
void Solider::shootBullet()
{
	Bullet* bullet = Bullet::create("Bullet.png");
	bullet->setUnitManager(_unitManager);
	bullet->setMap(_tiledMap);
	bullet->initWithTarget(this, _attackID);
}
void Solider::attackUpdate(float dt)
{
	if(_unitManager->_unitIdMap.count(_attackID)>0)//if (_unitManager->_unitIdMap[_attackID]!=nullptr)
	{
		_unitManager->createAttackMessage(_id, _attackID, _attackForce);
		//attack();
		/*if (_unitManager->_unitIdMap.count(_attackID)>0)//if (_unitManager->_unitIdMap[_attackID]!=nullptr)
		{
			_unitManager->_unitIdMap[_attackID]->getDamage(_attackForce);
			if (_unitManager->_unitIdMap[_attackID]->getCurrentHp() <= 0)
			{
				_unitManager->destoryUnit(_attackID);
				setAttackID(0);
				stopAttackUpdate();
			}
			else
				if (_unitManager->_unitIdMap[_attackID]->getCamp() == _unitManager->getPlayerCamp())
					_unitManager->_unitIdMap[_attackID]->displayHpBar();
		}
		else
			stopAttackUpdate();*/
	}
	else
		stopAttackUpdate();
}
void Solider::startAttackUpdate()
{
	this->schedule(schedule_selector(Solider::attackUpdate),0.5f);
}
void Solider::stopAttackUpdate()
{
	this->unschedule(schedule_selector(Solider::attackUpdate));
}
void Tank::autoAttack(float fd)
{
	if (_autoAttack == true && _underAttack == true && _attackID == 0)//如果敌人被毁灭，underattack改成false
	{
		if (_unitManager->_unitIdMap.count(_enermyId)>0//[_enermyId] != nullptr
			&& abs(_unitCoord._x - _unitManager->getUnitPos(_enermyId)._x) <= _autoAttackScope._width / 2
			&& abs(_unitCoord._y - _unitManager->getUnitPos(_enermyId)._y) <= _autoAttackScope._height / 2)
		{
			_attackID = _enermyId;
			_unitManager->attackingUnit[_id] = _enermyId;
			startAttackUpdate();
		}
		//if(_unitManager->_unitIdMap.count(_enermyId)>0)
		//_unitManager->newAttackUnit[_id] = _enermyId;
	}
	else
		if (_autoAttack == true && _attackID == 0)
		{
			int attackId = searchNearEnemy();
			if (attackId)
			{
				setAttackID(attackId);
				startAttackUpdate();
				//_unitManager->newAttackUnit[_id] = _attackID;
				//_unitManager->attackingUnit[_id] = _attackID;
				//
			}

		}
}
void Solider::autoAttack(float fd)
{
	if (_autoAttack == true && _underAttack == true && _attackID == 0)//如果敌人被毁灭，underattack改成false
	{
		if (_unitManager->_unitIdMap.count(_enermyId)>0//[_enermyId] != nullptr
			&& abs(_unitCoord._x - _unitManager->getUnitPos(_enermyId)._x) <= _autoAttackScope._width/2
			&& abs(_unitCoord._y - _unitManager->getUnitPos(_enermyId)._y) <= _autoAttackScope._height/2)
		{
			_attackID = _enermyId;
			_unitManager->attackingUnit[_id] = _enermyId;//
			startAttackUpdate();
		}
		//if(_unitManager->_unitIdMap.count(_enermyId)>0)
			//_unitManager->newAttackUnit[_id] = _enermyId;
	}
	else 
		if (_autoAttack == true && _attackID == 0)	
		{ 
			int attackId = searchNearEnemy();
			if (attackId)
			{
				setAttackID(attackId);
				startAttackUpdate();
				//_unitManager->newAttackUnit[_id] = _attackID;
				//_unitManager->attackingUnit[_id] = _attackID;
				//
			}

		}
}
/*void Solider::init(CampTypes camp, UnitTypes fightType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id)
{
	FightUnit::init()
}*/
