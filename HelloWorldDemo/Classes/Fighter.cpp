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
/*void Solider::init(CampTypes camp, UnitTypes fightType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap, int id)
{
	FightUnit::init()
}*/