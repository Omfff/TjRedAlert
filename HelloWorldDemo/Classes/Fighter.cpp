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
	_unitManager->createAttackMessage(_id, _attackID, _attackForce);
	attack();
	_unitManager->_unitIdMap[_attackID]->getDamage(_attackForce);
	_unitManager->_unitIdMap[_attackID]->displayHpBar();
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
	_unitManager->createAttackMessage(_id, _attackID, _attackForce);
	attack();
	_unitManager->_unitIdMap[_attackID]->getDamage(_attackForce);
	_unitManager->_unitIdMap[_attackID]->displayHpBar();
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
	if (_autoAttack == true && _underAttack == true && _attackID == 0)
	{
		_attackID = _enermyId;
		_unitManager->attackingUnit[_id] = _enermyId;
		startAttackUpdate();
		//if(_unitManager->_unitIdMap.count(_enermyId)>0)
		//_unitManager->newAttackUnit[_id] = _enermyId;
	}
	else
		if (_autoAttack == true && _attackID == 0)
		{
			if (searchNearEnemy())
			{
				_unitManager->newAttackUnit[_id] = _attackID;
				//_unitManager->attackingUnit[_id] = _attackID;
				//startAttackUpdate();
			}

		}
}
void Solider::autoAttack(float fd)
{
	if (_autoAttack == true && _underAttack == true && _attackID == 0)
	{
		_attackID = _enermyId;
		_unitManager->attackingUnit[_id] = _enermyId;
		startAttackUpdate();
		//if(_unitManager->_unitIdMap.count(_enermyId)>0)
			//_unitManager->newAttackUnit[_id] = _enermyId;
	}
	else 
		if (_autoAttack == true && _attackID == 0)
		{
			if (searchNearEnemy())
			{
				_unitManager->newAttackUnit[_id] = _attackID;
				//_unitManager->attackingUnit[_id] = _attackID;
				//startAttackUpdate();
			}

		}
}
/*void Solider::init(CampTypes camp, UnitTypes fightType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id)
{
	FightUnit::init()
}*/