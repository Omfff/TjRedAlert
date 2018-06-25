#include"Bullet.h"
Bullet* Bullet::create(const std::string& filename)
{
	Bullet* ret = new Bullet();
	if (ret && ret->initWithFile(filename)) 
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Bullet::initWithTarget(FightUnit* sender, int target)
{
	if (sender->getUnitType() == GI)
	{
		_speed = _GI_BULLET_SPEED;
	}
	else
	{
		_speed = _TANK_BULLET_SPEED;
	}
	setPosition(Point(sender->getUnitCoord()._x * 32+16, sender->getUnitCoord()._y * 32+16));// + Point(0, sender->getContentSize().height / 2));
	_tiledMap->addChild(this,11);
	_targetId = target;
	_target = _unitManager->getUnitPtr(target);
	_sender = sender;
	this->scheduleUpdate();
	return true;
}
void Bullet::update(float dt) 
{
	//_target = *m_targetPtr;[
	if (_unitManager->getUnitPtr(_targetId)==nullptr)
	{
		this->removeFromParentAndCleanup(true);
		return;
	}
	double distanceX = getPosition().x  - _target->getUnitCoord()._x*32;
	double distanceY = getPosition().y  - _target->getUnitCoord()._y*32;
	if (!(distanceX> _target->getUnitSize()._width * 32 * 0.25 && distanceX <= _target->getUnitSize()._width * 32 * 0.75
		&&distanceY > _target->getUnitSize()._height * 32 * 0.25 && distanceY <= _target->getUnitSize()._height * 32 * 0.75))
	{
		float distance = ccpDistance(getPosition(), _target->getPosition()); //+ Vec2(0, _target->getContentSize().height));
		float t = distance / _speed;
		float speed_x = (_target->getPositionX() - getPositionX()) / t;
		float speed_y = (_target->getPositionY() - getPositionY()) / t;
		setPositionX(getPositionX() + speed_x);
		setPositionY(getPositionY() + speed_y);
	}
	else 
	{
		this->removeFromParentAndCleanup(true);
	}
}
void Bullet::setMap(TMXTiledMap * map)
{
	_tiledMap = map;
}
void Bullet::setUnitManager(UnitManager* uM)
{
	_unitManager = uM;
}
