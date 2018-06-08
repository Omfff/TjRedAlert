#include"Bullet.h"
Bullet* Bullet::create(const std::string& filename)
{
	Bullet* ret = new Bullet();
	if (ret && ret->initWithFile(filename)) {
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
	setPosition(Point(sender->getUnitCoord()._x * 32, sender->getUnitCoord()._y * 32));// + Point(0, sender->getContentSize().height / 2));
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
	double distanceX = getPosition().x / 32.0 - _target->getUnitCoord()._x;
	double distanceY = getPosition().y / 32.0 - _target->getUnitCoord()._y;
	if (!(distanceX>=0&& distanceX <= _target->getUnitSize()._width&&distanceY >= 0 && distanceY <= _target->getUnitSize()._height))
	 {
		float distance = ccpDistance(getPosition(), _target->getPosition() + Vec2(0, _target->getContentSize().height));
		float t = distance / _speed;
		float speed_x = (_target->getPositionX() - getPositionX()) / t;
		float speed_y = (_target->getPositionY() + _target->getContentSize().height / 2 - getPositionY()) / t;
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
