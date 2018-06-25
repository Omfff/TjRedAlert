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
	if (camp == _unitManager->getPlayerCamp())
		this->schedule(schedule_selector(Solider::autoAttack),0.1f);
	return true;
}
bool Tank::init(CampTypes camp, UnitTypes types, GridVec2 coord,
	TMXTiledMap* map, GridMap * gridmap, int id)
{
	FightUnit::init(camp, types, coord, map, gridmap, id);
	if (camp == _unitManager->getPlayerCamp())
		this->schedule(schedule_selector(Solider::autoAttack),0.1f);
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
	if(_camp==_unitManager->getPlayerCamp())
		SimpleAudioEngine::getInstance()->playEffect("Music/Tank attack.wav");//音效
	//SimpleAudioEngine::getInstance()->stopEffect
}
void Tank::attackUpdate(float dt)
{
	GridVec2 atkRectPoint = GridVec2((_unitCoord._x - _autoAttackScope._width / 2)< 0 ? 0 : (_unitCoord._x - _autoAttackScope._width / 2),
		(_unitCoord._y - _autoAttackScope._height / 2) < 0 ? 0 : (_unitCoord._y - _autoAttackScope._height / 2));
	GridDimen attackSize;
	if (_unitCoord._x + _autoAttackScope._width / 2 >= _MAP_WIDTH)
		attackSize._width = _autoAttackScope._width - ((_unitCoord._x + _autoAttackScope._width / 2) - _MAP_WIDTH + 1);
	else
		attackSize._width = _autoAttackScope._width;
	if (_unitCoord._y + _autoAttackScope._height / 2 >= _MAP_HEIGHT)
		attackSize._height = _autoAttackScope._height - ((_unitCoord._y + _autoAttackScope._height / 2) - _MAP_HEIGHT + 1);
	else
		attackSize._height = _autoAttackScope._height;
	auto  atkRect = GridRect(atkRectPoint, attackSize);
	if (_unitManager->_unitIdMap.count(_attackID)>0
		&& atkRect.intersectsRect(_unitManager->_unitIdMap[_attackID]->getUnitRect()))//if (_unitManager->_unitIdMap[_attackID]!=nullptr)
	{
		_unitManager->createAttackMessage(_id, _attackID, _attackForce);
	}
	else
		stopAttackUpdate();
}
void Tank::startAttackUpdate()
{
	this->schedule(schedule_selector(Tank::attackUpdate), 0.75f);
}
void Tank::stopAttackUpdate()
{
	this->unschedule(schedule_selector(Tank::attackUpdate));
	setAttackID(0);
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
	if (_camp == _unitManager->getPlayerCamp())
		SimpleAudioEngine::getInstance()->playEffect("Music/Tank attack.wav");
}
void Solider::attackUpdate(float dt)
{
	GridVec2 atkRectPoint = GridVec2((_unitCoord._x - _autoAttackScope._width / 2)< 0 ? 0 : (_unitCoord._x - _autoAttackScope._width / 2),
		(_unitCoord._y - _autoAttackScope._height / 2) < 0 ? 0 : (_unitCoord._y - _autoAttackScope._height / 2));
	GridDimen attackSize;
	if (_unitCoord._x + _autoAttackScope._width / 2 >= _MAP_WIDTH)
		attackSize._width = _autoAttackScope._width - ((_unitCoord._x + _autoAttackScope._width / 2) - _MAP_WIDTH + 1);
	else
		attackSize._width = _autoAttackScope._width;
	if (_unitCoord._y + _autoAttackScope._height / 2 >= _MAP_HEIGHT)
		attackSize._height = _autoAttackScope._height - ((_unitCoord._y + _autoAttackScope._height / 2) - _MAP_HEIGHT + 1);
	else
		attackSize._height = _autoAttackScope._height;
	auto  atkRect = GridRect(atkRectPoint, attackSize);
	
	if(_unitManager->_unitIdMap.count(_attackID)>0
		&&atkRect.intersectsRect(_unitManager->_unitIdMap[_attackID]->getUnitRect()))//if (_unitManager->_unitIdMap[_attackID]!=nullptr)
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
	setAttackID(0);
}
void Tank::autoAttack(float fd)
{
	if (_attackID != 0 && _unitManager->_unitIdMap.count(_attackID) == 0)
		_attackID = 0;
	if (_enermyId != 0 && _underAttack == true && _unitManager->_unitIdMap.count(_enermyId) == 0)
		_enermyId = 0;
	if (_autoAttack == true && _underAttack == true && _attackID == 0)//如果敌人被毁灭，underattack改成false
	{
		if (_unitManager->_unitIdMap.count(_enermyId)>0//[_enermyId] != nullptr
			&& abs(_unitCoord._x - _unitManager->getUnitPos(_enermyId)._x) <= _autoAttackScope._width / 2
			&& abs(_unitCoord._y - _unitManager->getUnitPos(_enermyId)._y) <= _autoAttackScope._height / 2
			&& _unitManager->_unitIdMap[_enermyId]->getActionManager()->getNumberOfRunningActionsInTarget(_unitManager->_unitIdMap[_enermyId]) == 0)
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
	if (_attackID != 0 && _unitManager->_unitIdMap.count(_attackID) == 0)
		_attackID = 0;
	if (_enermyId != 0 && _underAttack == true && _unitManager->_unitIdMap.count(_enermyId) == 0)
		_enermyId = 0;
	if (_autoAttack == true && _underAttack == true && _attackID == 0)//如果敌人被毁灭，underattack改成false
	{
		if (_unitManager->_unitIdMap.count(_enermyId)>0//[_enermyId] != nullptr
			&& abs(_unitCoord._x - _unitManager->getUnitPos(_enermyId)._x) <= _autoAttackScope._width/2
			&& abs(_unitCoord._y - _unitManager->getUnitPos(_enermyId)._y) <= _autoAttackScope._height/2
			&&_unitManager->_unitIdMap[_enermyId]->getActionManager()->getNumberOfRunningActionsInTarget(_unitManager->_unitIdMap[_enermyId])==0)
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
AttackDog* AttackDog::create(const std::string& filename)
{
	AttackDog *ret = new (std::nothrow) AttackDog();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
bool AttackDog::init(CampTypes camp, UnitTypes types, GridVec2 coord,
	TMXTiledMap* map, GridMap * gridmap, int id)
{
	FightUnit::init(camp, types, coord, map, gridmap, id);
	if (camp == _unitManager->getPlayerCamp())
		this->schedule(schedule_selector(AttackDog::autoAttack),0.1f);
	return true;
}
void AttackDog::autoAttack(float fd)
{
	if (_attackID != 0 && _unitManager->_unitIdMap.count(_attackID) == 0)
		_attackID = 0;
	if (_enermyId != 0 && _underAttack == true && _unitManager->_unitIdMap.count(_enermyId) == 0)
		_enermyId = 0;
	if (_autoAttack == true && _underAttack == true && _attackID == 0)//如果敌人被毁灭，underattack改成false
	{
		if (_unitManager->_unitIdMap.count(_enermyId)>0//[_enermyId] != nullptr
			&& abs(_unitCoord._x - _unitManager->getUnitPos(_enermyId)._x) <= _autoAttackScope._width / 2
			&& abs(_unitCoord._y - _unitManager->getUnitPos(_enermyId)._y) <= _autoAttackScope._height / 2
			&& _unitManager->_unitIdMap[_enermyId]->getActionManager()->getNumberOfRunningActionsInTarget(_unitManager->_unitIdMap[_enermyId]) == 0)
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
void AttackDog::stopAttackUpdate()
{
	this->unschedule(schedule_selector(AttackDog::attackUpdate));
	setAttackID(0);
}
void AttackDog::startAttackUpdate()
{
	this->schedule(schedule_selector(AttackDog::attackUpdate), 0.25f);
}
void AttackDog::attackUpdate(float dt)
{
	GridVec2 atkRectPoint = GridVec2((_unitCoord._x - _autoAttackScope._width / 2)< 0 ? 0 : (_unitCoord._x - _autoAttackScope._width / 2),
		(_unitCoord._y - _autoAttackScope._height / 2) < 0 ? 0 : (_unitCoord._y - _autoAttackScope._height / 2));
	GridDimen attackSize;
	if (_unitCoord._x + _autoAttackScope._width / 2 >= _MAP_WIDTH)
		attackSize._width = _autoAttackScope._width - ((_unitCoord._x + _autoAttackScope._width / 2) - _MAP_WIDTH + 1);
	else
		attackSize._width = _autoAttackScope._width;
	if (_unitCoord._y + _autoAttackScope._height / 2 >= _MAP_HEIGHT)
		attackSize._height = _autoAttackScope._height - ((_unitCoord._y + _autoAttackScope._height / 2) - _MAP_HEIGHT + 1);
	else
		attackSize._height = _autoAttackScope._height;
	auto  atkRect = GridRect(atkRectPoint, attackSize);

	if (_unitManager->_unitIdMap.count(_attackID)>0
		&& atkRect.intersectsRect(_unitManager->_unitIdMap[_attackID]->getUnitRect())
		&&( _unitManager->_unitIdMap[_attackID]->getUnitType()==GI || _unitManager->_unitIdMap[_attackID]->getUnitType() == ATTACKDOG))//if (_unitManager->_unitIdMap[_attackID]!=nullptr)
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
void AttackDog::attack()
{
	/*Animation* animation = Animation::create();
	for (int i = 1; i <= 4; i++)
	{
		__String *frameName = __String::createWithFormat("", i);
		log("frameName = %s", frameName->getCString());
		SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
		animation->addSpriteFrame(spriteFrame);
	}

	animation->setDelayPerUnit(0.15f);     //设置两个帧播放时间
	animation->setLoops(1);
	animation->setRestoreOriginalFrame(true);    //动画执行后还原初始状态

	Animate* action = Animate::create(animation);
	this->runAction(action);*/
	//if (_unitManager->_unitIdMap.count(_attackID) > 0)
	
		/*GridVec2 attackIdPos = _unitManager->_unitIdMap[_attackID]->getUnitCoord();
		Vec2 currentPos = Vec2(_unitCoord._x * 32 + 16, _unitCoord._y * 32 + 16);
		FiniteTimeAction* ac1 = (FiniteTimeAction*)this->runAction(JumpTo::create(2, Vec2(attackIdPos._x * 32+16, attackIdPos._y * 32+16), 6, 1));
		FiniteTimeAction* ac2 = (FiniteTimeAction*)this->runAction(Place::create(currentPos));
		this->runAction(Sequence::create(ac2, ac1, NULL));	//////////////////动画结束///////////////////*/
	if (_camp == _unitManager->getPlayerCamp())
		SimpleAudioEngine::getInstance()->playEffect("Music/dogAttack.wav");
	this->runAction(RotateBy::create(0.25f, 360));
	
}