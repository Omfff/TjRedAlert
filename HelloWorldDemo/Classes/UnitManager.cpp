
#include"UnitManager.h"
#include"GameMessages.pb.h"
#include"Building.h"
#include"Unit.h"
#include"Fighter.h"
#include<vector>
#include<string>
USING_NS_CC;
using namespace std;
#define MESSAGECRT GameMessage::Cmd::GameMessage_Cmd_CRT 
#define MESSAGEEMP GameMessage::Cmd::GameMessage_Cmd_EMP 
#define MESSAGEMOV GameMessage::Cmd::GameMessage_Cmd_MOV
#define MESSAGEATK GameMessage::Cmd::GameMessage_Cmd_ATK

bool UnitManager::init()
{
	
	return true;
}
void UnitManager::updateUnitState()
{
	int receiveMessNum = _msgGroup->game_message_size();
	if (receiveMessNum == 0)
		log("receive empty message");
	else
	{
		for (int i = 0; i < receiveMessNum; i++)
		{
			auto receiveMessage=_msgGroup->game_message(i);
			if (receiveMessage.cmd() == MESSAGECRT)
			{
				int id = receiveMessage.unit_id1();
				GridVec2  pos = GridVec2(receiveMessage.position().x(), receiveMessage.position().y());
				Unit * newUnit = creatUnit( CampTypes(receiveMessage.camp()), UnitTypes(receiveMessage.unit_type()),pos, receiveMessage.unit_id1());
				//_unitIdMap[id] = newUnit;
			}
			else if (receiveMessage.cmd() == MESSAGEMOV)
			{
				int id = receiveMessage.unit_id1();
				if(_unitIdMap.count(id)>0)//if (_unitIdMap[id]!=nullptr)
				{
					Unit * unit = _unitIdMap[id];
<<<<<<< HEAD
					auto messageMovePath = receiveMessage.path();
					vector<GridVec2> movePath;
					int pathPointNum = messageMovePath.path_point_size();
					for (int i = 0; i < pathPointNum; i++)
					{
						GridVec2 point;
						auto pathPoint = messageMovePath.path_point(i);
						point._x = pathPoint.x();
						point._y = pathPoint.y();
						movePath.push_back(point);
					}
					unit->setGridPath(movePath);
=======
					unit->setDestination(Vec2(receiveMessage.position().x()*32.0,receiveMessage.position().y()*32.0));//è¿™ä¸ªpositionçš„å‚æ•°éƒ½æ˜¯intçš„
>>>>>>> 9ae34c7ea39cffbed1688d2060bfea7903ee8fa4
					unit->move();
				}
				else
				{
					log("empty _unitIdMap doesn't contain receiveMessage.unit_id1 ,the cmd mov is wrong !");
				}
			}
			else
			{
				if (receiveMessage.cmd() == MESSAGEATK)//unit1æ”¶åˆ°unit2æ”»å‡»
				{
					int unitId1 = receiveMessage.unit_id1();
					int unitId2 = receiveMessage.unit_id2();
					int damage = receiveMessage.damage();
					_unitIdMap[unitId1]->attack();
					if (_unitIdMap.count(unitId2)>0)//if (_unitManager->_unitIdMap[_attackID]!=nullptr)
					{
						_unitIdMap[unitId2]->getDamage(damage);
						if (_unitIdMap[unitId2]->getCurrentHp() <= 0)
						{
							destoryUnit(unitId2);
							_unitIdMap[unitId1]->setAttackID(0);
							_unitIdMap[unitId1]->stopAttackUpdate();
						}
						else
							if (_unitIdMap[unitId2]->getCamp() == getUnitCamp(unitId2))
							{
								_unitIdMap[unitId2]->setEnermyId(unitId1);//×Ô¶¯¹¥»÷
								_unitIdMap[unitId2]->setUnderAttack(true);//×Ô¶¯¹¥»÷
								_unitIdMap[unitId2]->displayHpBar();
							}
					}
					else
						_unitIdMap[unitId1]->stopAttackUpdate();
				}
			}
		}
	}
	_msgGroup->clear_game_message();
}
void UnitManager::destoryUnit(int id)
{
	if(_unitIdMap.count(id)>0)//if (_unitIdMap[id] != nullptr)
	{
<<<<<<< HEAD
		Unit * unit = _unitIdMap[id];
		if (unit)
		{
			if (unit->getUnitType() == WARFACTORY)
				_warFactoryId.erase(unit->getID());
			if (unit->getUnitType() == BARRACKS)
				_barracksId.erase(unit->getID());
			if (unit->getUnitType() >= 5 && unit->getAttackID() != 0)
			{
				int attackId = unit->getAttackID();
				if (_unitIdMap.count(attackId)>0&&_unitIdMap[attackId]->getCamp() == _playerCamp)//_unitIdMap[attackId] != nullptr &&
				{
					_unitIdMap[attackId]->setEnermyId(0);
					_unitIdMap[attackId]->setUnderAttack(false);
				}
			}
			if (_selectedUnitID.count(id) > 0)
				_selectedUnitID.erase(id);
			unit->removeFromMap();
			//unit->deleteUnit();
			_tileMap->removeChild(unit);
			_unitIdMap.erase(id);

		}
=======
		if (unit->getUnitType() == WARFACTORY) 
			_warFactoryId.erase(unit->getID());
		if (unit->getUnitType() == BARRACKS)
			_barracksId.erase(unit->getID());
		unit->removeFromMap();
		SimpleAudioEngine::getInstance()->playEffect("Music/Unit lost.wav");//éŸ³æ•ˆ
		//unit->deleteUnit();
		_tileMap->removeChild(unit);
		_unitIdMap.erase(id);	
		
>>>>>>> 9ae34c7ea39cffbed1688d2060bfea7903ee8fa4
	}
}
//BASE ,POWERPLANT,BARRACKS,WARFACTORY,OREREFINERY,GI,ATTACKDOG,TANK
Unit * UnitManager::creatUnit(CampTypes camp, UnitTypes type, const  GridVec2& pos, int id )
{
	if (id == 0)
		id = _nextId;
	Unit * unit;
	vector<string >picPath = {"units/Base(","units/PowerPlant(","units/Barrack(","units/WarFactory(","units/OreRefinery(","","",""};
	vector<string>picColor = { "","red","blue","green","yellow" };
	
	int transType = type;
	string unitPic = picPath[type] + picColor[camp] + ").png";
	switch (type)
	{
		case BASE:
			unit = Base::create(unitPic);
			//SimpleAudioEngine::getInstance()->playEffect("Music/Construction complete.wav");//éŸ³æ•ˆ
			break;
		case POWERPLANT:
			unit = PowerPlant::create(unitPic);
			//SimpleAudioEngine::getInstance()->playEffect("Music/Construction complete.wav");//éŸ³æ•ˆ
			break;
		case BARRACKS:
			_barracksId[id]=0;
			unit = Barracks::create(unitPic);
			//SimpleAudioEngine::getInstance()->playEffect("Music/Construction complete.wav");//éŸ³æ•ˆ
			break;
		case WARFACTORY:
			_warFactoryId[id]=0;
			unit = WarFactory::create(unitPic);
			//SimpleAudioEngine::getInstance()->playEffect("Music/Construction complete.wav");//éŸ³æ•ˆ
			break;
		case OREREFINERY:
			unit = OreRefinery::create(unitPic);
			//SimpleAudioEngine::getInstance()->playEffect("Music/Construction complete.wav");//éŸ³æ•ˆ
			break;
		case GI:
			unit = Solider::create("solider.png");
			//SimpleAudioEngine::getInstance()->playEffect("Music/Unit complete.wav");//éŸ³æ•ˆ
			break;
		case ATTACKDOG:
			//SimpleAudioEngine::getInstance()->playEffect("Music/Unit complete.wav");//éŸ³æ•ˆ
			break;
		case TANK:
			//SimpleAudioEngine::getInstance()->playEffect("Music/Unit complete.wav");//éŸ³æ•ˆ
			break;
		default:
			break;
	}
	unit->init( camp, type, pos, _tileMap, _gridMap,id);
	unit->setUnitManager(this);
	_unitIdMap[id] = unit;
	unit->setPosition(Vec2(pos._x, pos._y));
	_tileMap->addChild(unit, 10);
	return unit;
}
GridVec2 UnitManager::getUnitPos(int id)
{
	if (_unitIdMap.count(id) > 0)
	{
		Unit *unit = _unitIdMap[id];
		if (unit)
			return unit->getUnitCoord();
		else
			return GridVec2(-1, -1);
	}
	else
		return GridVec2(-1, -1);
}
CampTypes UnitManager::getUnitCamp(int id)
{
	if (_unitIdMap.count(id))
	{
		Unit * unit = _unitIdMap[id];
		if (unit)
			return (unit->getCamp());
		else
			return NULLCAMP;
	}
	else
		return NULLCAMP;
}
void UnitManager::selectUnits(const GridRect &range)
{
	deselectAllUnits();
	set<int> choosedUnitId=_gridMap->getUnitIdAt(range);
	for (auto id : choosedUnitId)
	{
		if (_unitIdMap[id]->getCamp() == _playerCamp)
		{
			_selectedUnitID.insert(id);
			_unitIdMap[id]->displayHpBar();//hpbar
		}
	}
}
void UnitManager::deselectAllUnits()
{
	if (!_selectedUnitID.empty())
	{
		for (auto unitid : _selectedUnitID)
			_unitIdMap[unitid]->hideHpBar();//hpbar
		_selectedUnitID.clear();
	}
}
void UnitManager::creatProduceMessage(UnitTypes unitType,const GridVec2 &pos)
{
	auto newMessage = _msgGroup->add_game_message();
	newMessage->set_camp(_playerCamp);
	newMessage->set_cmd(MESSAGECRT);
	newMessage->set_unit_type(unitType);
	newMessage->set_unit_id1(_nextId);//å•ä½idçš„è®¾ç½®ï¼Ÿï¼Ÿï¼Ÿ
	_nextId += 4;
	GridPoint * posPoint =newMessage->mutable_position();
	posPoint->set_x(  pos._x);
	posPoint->set_y( pos._y);
	//æ€ä¹ˆè®¾ç½®åæ ‡ï¼Ÿï¼Ÿ
}
void UnitManager::creatMoveMessage(int id, vector<GridVec2> &path )
{
	auto newMessage = _msgGroup->add_game_message();
	newMessage->set_camp(_playerCamp);
	newMessage->set_cmd(MESSAGEMOV);
	newMessage->set_unit_id1(id);
	GridPath * movePath = newMessage->mutable_path();
	for (auto pathPoint : path)
	{
		auto pointPos = movePath->add_path_point();
		pointPos->set_x(pathPoint._x);
		pointPos->set_y(pathPoint._y);
	}
	//GridPoint * posPoint = newMessage->mutable_position();
	//posPoint->set_x(pos.x);
	//posPoint->set_y(pos.y);
}
void UnitManager::createAttackMessage(int id1, int id2, int damage)
{
	auto newMessage = _msgGroup->add_game_message();
	newMessage->set_camp(_playerCamp);
	newMessage->set_cmd(MESSAGEATK);
	newMessage->set_unit_id1(id1);
	newMessage->set_unit_id2(id2);
	newMessage->set_damage(damage);
}
//æœ¬åœ°ç§»åŠ¨å’Œæ”»å‡»çš„æ¶ˆæ¯æ€ä¹ˆç”Ÿæˆï¼Ÿï¼Ÿ
void UnitManager::choosePosOrUnit(const GridVec2 & pos)
{
	int idAtPos = _gridMap->getUnitIdAt(GridVec2(pos._x / 32, pos._y / 32));
	if (!_selectedUnitID.empty())
	{
		if (idAtPos == 0||idAtPos ==_NO_PASS)
		{
			for (auto unitid : _selectedUnitID)
			{
				if (_unitIdMap[unitid]->getUnitType()>=5)//æ˜¯å¯ç§»åŠ¨å•ä½
				{
<<<<<<< HEAD
					_unitIdMap[unitid]->stopAttackUpdate();
					_unitIdMap[unitid]->setAttackID(0);
					if (destination != _unitIdMap[unitid]->getDestination()) 
					{
						_unitIdMap[unitid]->stopAllActions();
						GridVec2 unitCoord(_unitIdMap[unitid]->getPosition().x / 32, _unitIdMap[unitid]->getPosition().y / 32);
						GridRect unitRect(GridRect(GridVec2(_unitIdMap[unitid]->getPosition().x / 32, _unitIdMap[unitid]->getPosition().y / 32), GridDimen(1, 1)));
						_unitIdMap[unitid]->_battleMap->unitLeavePosition(_unitIdMap[unitid]->getUnitRect());
						_unitIdMap[unitid]->setUnitCoord(unitCoord);
						_unitIdMap[unitid]->setUnitRect(unitRect);
						_unitIdMap[unitid]->_battleMap->unitCoordStore(_unitIdMap[unitid]->getID(), unitRect);
					}

					_unitIdMap[unitid]->setDestination(destination);
					_unitIdMap[unitid]->tryToFindPath();
					//_unitIdMap[unitid]->move();
					//²úÉúÒÆ¶¯ÏûÏ¢
					/*if (attackingUnit.count(unitid) > 0)
=======
					//å¯»è·¯
					//äº§ç”Ÿç§»åŠ¨æ¶ˆæ¯
					if (attackingUnit.count(unitid) > 0)
>>>>>>> 9ae34c7ea39cffbed1688d2060bfea7903ee8fa4
					{
						_unitIdMap[unitid]->stopAttackUpdate();
						_unitIdMap[unitid]->setAttackID(0);
					}
						//unschedule(schedule_selector(_unitIdMap[unitid]->attackUpdate));
					_unitIdMap[unitid]->setDestination(Vec2(pos._x, pos._y));
					_unitIdMap[unitid]->move();
				}
			}
		}
		else
		{
			if (_unitIdMap[idAtPos]->getCamp()==_playerCamp)//é€‰æ‹©äº†æˆ‘æ–¹çš„å•ä½ åˆ™æ˜¾ç¤ºè¡€æ¡
			{
				/*deselectAllUnits();
				_selectedUnitID.push_back(idAtPos);
				_unitIdMap[idAtPos]->displayHpBar();*/
				if (_unitIdMap[idAtPos]->getUnitType() >=0)
				{
					for (auto unitid : _selectedUnitID)
					{
						if (_unitIdMap[unitid]->getUnitType() >= 5)
						{
							int distanceX = abs(getUnitPos(idAtPos)._x - getUnitPos(unitid)._x);
							int distanceY = abs(getUnitPos(idAtPos)._y - getUnitPos(unitid)._y);
							if ( distanceX > AUTO_ATTACK_RANGE[_unitIdMap[unitid]->getUnitType() - 5]._width / 2
								||  distanceY > AUTO_ATTACK_RANGE[_unitIdMap[unitid]->getUnitType() - 5]._height / 2)
							{
								//ÒÆ¶¯µ½½¨Öşµ¥Î»¸½½ü
								_unitIdMap[unitid]->stopAttackUpdate();
								_unitIdMap[unitid]->setAttackID(idAtPos);
								if (destination != _unitIdMap[unitid]->getDestination())
								{
									_unitIdMap[unitid]->stopAllActions();
									GridVec2 unitCoord(_unitIdMap[unitid]->getPosition().x / 32, _unitIdMap[unitid]->getPosition().y / 32);
									GridRect unitRect(GridRect(GridVec2(_unitIdMap[unitid]->getPosition().x / 32, _unitIdMap[unitid]->getPosition().y / 32), GridDimen(1, 1)));
									_unitIdMap[unitid]->_battleMap->unitLeavePosition(_unitIdMap[unitid]->getUnitRect());
									_unitIdMap[unitid]->setUnitCoord(unitCoord);
									_unitIdMap[unitid]->setUnitRect(unitRect);
									_unitIdMap[unitid]->_battleMap->unitCoordStore(_unitIdMap[unitid]->getID(), unitRect);
								}

								_unitIdMap[unitid]->setDestination(destination);
								_unitIdMap[unitid]->tryToFindPath();
								_unitIdMap[unitid]->move();
							}
							_unitIdMap[unitid]->setAttackID(idAtPos);
							_unitIdMap[unitid]->startAttackUpdate();
						}
					}
				}
			}
			else////é€‰æ‹©äº†æ•Œæ–¹å•ä½
			{
<<<<<<< HEAD
				//1µĞ·½½¨Öş µ½´ï ¹¥»÷
				//2µĞ·½±øÖÖ ¸ú×Ù ¹¥»÷
				
				if (_unitIdMap[idAtPos]->getUnitType() < 5)
				{
					for (auto unitid : _selectedUnitID)
					{
						if (_unitIdMap[unitid]->getUnitType() >= 5)
						{
							if (abs(getUnitPos(idAtPos)._x - getUnitPos(unitid)._x) > AUTO_ATTACK_RANGE[_unitIdMap[unitid]->getUnitType() - 5]._width / 2
								&& abs(getUnitPos(idAtPos)._y - getUnitPos(unitid)._y) > AUTO_ATTACK_RANGE[_unitIdMap[unitid]->getUnitType() - 5]._height / 2)
							{
								//ÒÆ¶¯µ½½¨Öşµ¥Î»¸½½ü
								_unitIdMap[unitid]->stopAttackUpdate();
								_unitIdMap[unitid]->setAttackID(idAtPos);
								if (destination != _unitIdMap[unitid]->getDestination())
								{
									_unitIdMap[unitid]->stopAllActions();
									GridVec2 unitCoord(_unitIdMap[unitid]->getPosition().x / 32, _unitIdMap[unitid]->getPosition().y / 32);
									GridRect unitRect(GridRect(GridVec2(_unitIdMap[unitid]->getPosition().x / 32, _unitIdMap[unitid]->getPosition().y / 32), GridDimen(1, 1)));
									_unitIdMap[unitid]->_battleMap->unitLeavePosition(_unitIdMap[unitid]->getUnitRect());
									_unitIdMap[unitid]->setUnitCoord(unitCoord);
									_unitIdMap[unitid]->setUnitRect(unitRect);
									_unitIdMap[unitid]->_battleMap->unitCoordStore(_unitIdMap[unitid]->getID(), unitRect);
								}

								_unitIdMap[unitid]->setDestination(destination);
								_unitIdMap[unitid]->tryToFindPath();
								_unitIdMap[unitid]->move();
							}
							_unitIdMap[unitid]->setAttackID(idAtPos);
							_unitIdMap[unitid]->startAttackUpdate();
							//_unitIdMap[idAtPos]->setEnermyId(unitid);//²âÊÔ×Ô¶¯¹¥»÷
							//_unitIdMap[idAtPos]->setUnderAttack(true);//²âÊÔ×Ô¶¯¹¥»÷
						}
					}
				}
				else
				{
					//2µĞ·½±øÖÖ ¸ú×Ù ¹¥»÷
				}
				
=======
				//1æ•Œæ–¹å»ºç­‘ åˆ°è¾¾ æ”»å‡»
				//2æ•Œæ–¹å…µç§ è·Ÿè¸ª æ”»å‡»
				/*for (auto unitid : _selectedUnitID)
				{
					if (_unitIdMap[unitid]->getUnitType() >= 5)//æ˜¯å¯æ”»å‡»å•ä½
					{
						_unitIdMap[unitid]->setAttackID(idAtPos);
						_unitIdMap[unitid]->attack();
						_unitIdMap[idAtPos]->getDamage(100);
						_unitIdMap[idAtPos]->displayHpBar();
						if (_unitIdMap[idAtPos]->getCurrentHp() == 0)
						{
							destoryUnit(idAtPos);
						}
					}
				}*/
>>>>>>> 9ae34c7ea39cffbed1688d2060bfea7903ee8fa4
			}
		}
	}
}
/*void UnitManager::unitAttackUpdate()
{
	if (!newAttackUnit.empty())
	{
		for (auto battleUnit : newAttackUnit)
		{
			if (attackingUnit.count(battleUnit.first) > 0&& attackingUnit[battleUnit.first] == battleUnit.second)
				continue;

			if (attackingUnit.count(battleUnit.first) > 0 && attackingUnit[battleUnit.first] != battleUnit.second)
				_unitIdMap[battleUnit.first]->stopAttackUpdate();
				//this->unschedule(schedule_selector(_unitIdMap[battleUnit.first]->attackUpdate));
				
			attackingUnit[battleUnit.first] = battleUnit.second;
			_unitIdMap[battleUnit.first]->setAttackID(battleUnit.second);
			_unitIdMap[battleUnit.first]->startAttackUpdate();
			//this->schedule(schedule_selector(_unitIdMap[battleUnit.first]->attackUpdate),0.5f);
		}
		newAttackUnit.clear();
	}
	if (!attackingUnit.empty())
	{
		map<int, int> ::iterator ite = attackingUnit.begin();
		vector<int> deleUnitId;
		while (ite != attackingUnit.end())
		{
			if (_unitIdMap[ite->second] != nullptr&&_unitIdMap[ite->second]->getCurrentHp() <= 0)
			{
				_unitIdMap[ite->first]->stopAttackUpdate();
				//this->unschedule(schedule_selector(_unitIdMap[battleUnit.first]->attackUpdate));
				_unitIdMap[ite->first]->setAttackID(0);
				destoryUnit(ite->second);
				ite = attackingUnit.erase(ite);
				//deleUnitId.push_back(ite->first);
			}
			else if (_unitIdMap[ite->second] == nullptr)
			{
				_unitIdMap[ite->first]->stopAttackUpdate();
				//this->unschedule(schedule_selector(_unitIdMap[battleUnit.first]->attackUpdate));
				_unitIdMap[ite->first]->setAttackID(0);
				ite = attackingUnit.erase(ite);
			}
			else
			{
				ite++;
			}
		}
	}
<<<<<<< HEAD
}*/
=======

}
>>>>>>> 9ae34c7ea39cffbed1688d2060bfea7903ee8fa4
void UnitManager::fighterUnitProductionUpdate()
{
	auto ite = _fighterProduceSeq.begin();
	int mark = 0;
	while (ite != _fighterProduceSeq.end())
	{
		if (*ite==GI&& _barracksId.empty() != true)
		{
			for (auto & id : _barracksId)
			{
				if (id.second == 0)
				{
					GridVec2 producePos = _unitIdMap[id.first]->findEmptyPosToProduceSolider();
					creatProduceMessage(GI, GridVec2(producePos._x / 32, producePos._y/32));
					creatUnit(_playerCamp, GI, producePos);
					_waitingGINum--;
					id.second = 1;
					mark = 1;
					ite = _fighterProduceSeq.erase(ite);
					break;
				}
			}
		}
		else if(*ite==ATTACKDOG&& _barracksId.empty() != true)
		{
			for (auto & id : _barracksId)
			{
				if (id.second == 0)
				{
					GridVec2 producePos = _unitIdMap[id.first]->findEmptyPosToProduceSolider();
					creatProduceMessage(ATTACKDOG, GridVec2(producePos._x / 32, producePos._y/32));
					creatUnit(_playerCamp, ATTACKDOG, producePos);
					_waitingAttackDogNum--;
					id.second = 1;
					mark = 1;
					ite = _fighterProduceSeq.erase(ite);
					break;
				}
			}
		}
		else //(*ite == TANK && _warFactoryId.empty() != true)
		{
			if (*ite == TANK && _warFactoryId.empty() != true)
			{
				for (auto & id : _warFactoryId)
				{
					if (id.second == 0)
					{
						GridVec2 producePos = _unitIdMap[id.first]->findEmptyPosToProduceTank();
						creatProduceMessage(TANK, GridVec2(producePos._x / 32, producePos._y / 32));
						creatUnit(_playerCamp, TANK, producePos);
						_waitingTankNum--;
						id.second = 1;
						mark = 1;
						ite = _fighterProduceSeq.erase(ite);
						break;
					}
				}
			}
		}
		if (mark == 1)
			mark = 0;
		else
			ite++;
	}
}
GridRect transferRectToGridRect(const Rect & rect)
{
	return GridRect(GridVec2((float)(rect.origin.x) / 32.0, (float)(rect.origin.y / 32.0)),
		GridDimen(rect.size.width/32.0, rect.size.height/32.0));
}
Unit * UnitManager::getUnitPtr(int id)
{
	if (_unitIdMap.count(id) > 0)
		return _unitIdMap[id];
	else
		return nullptr;
}
void UnitManager::onEnter() {
	Layer::onEnter();
	SimpleAudioEngine::getInstance()->preloadEffect("Music/Unit lost.wav");
	
}
void UnitManager::onExit() {
	Layer::onExit();
	SimpleAudioEngine::getInstance()->stopAllEffects();
}
