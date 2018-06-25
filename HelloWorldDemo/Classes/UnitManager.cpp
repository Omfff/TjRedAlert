
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
#define MESSAGECHT GameMessage::Cmd::GameMessage_Cmd_CHT

bool UnitManager::init()
{
	_endFlag.insert(pair<CampTypes, bool>(RED, false));
	_endFlag.insert(pair<CampTypes, bool>(BLUE, false));
	_endFlag.insert(pair<CampTypes, bool>(GREEN, false));
	_endFlag.insert(pair<CampTypes, bool>(YELLOW, false));
	return true;
}
void UnitManager::updateUnitState()
{
	auto sendMessage = _msgGroup->SerializeAsString();
	_socketClient->do_write_string(sendMessage);
	auto receiveMessageGroup = _socketClient->get_string_from_server();
	_msgGroup->ParseFromString(receiveMessageGroup);
	int receiveMessNum = _msgGroup->game_message_size();
	if (receiveMessNum == 0)
		log("receive empty message");
	else
	{
		for (int i = 0; i < receiveMessNum; i++)
		{
			auto receiveMessage = _msgGroup->game_message(i);
			if (receiveMessage.cmd() == MESSAGECRT)
			{
				int id = receiveMessage.unit_id1();
				GridVec2  pos = GridVec2(receiveMessage.position().x(), receiveMessage.position().y());
				Unit * newUnit = creatUnit(CampTypes(receiveMessage.camp()), UnitTypes(receiveMessage.unit_type()), pos, receiveMessage.unit_id1());
			}
			else if (receiveMessage.cmd() == MESSAGEMOV)
			{
				int id = receiveMessage.unit_id1();
				if (_unitIdMap.count(id)>0)//if (_unitIdMap[id]!=nullptr)
				{
					Unit * unit = _unitIdMap[id];
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
					if (unit->getActionManager()->getNumberOfRunningActionsInTarget(unit))
						unit->stopAllActions();
					unit->move();
				}
				else
				{
					log("empty _unitIdMap doesn't contain receiveMessage.unit_id1 ,the cmd mov is wrong !");
				}
			}

			else if (receiveMessage.cmd() == MESSAGEATK)//unit1收到unit2攻击
			{
				int unitId1 = receiveMessage.unit_id1();
				int unitId2 = receiveMessage.unit_id2();
				int damage = receiveMessage.damage();
				if (_unitIdMap.count(unitId1) > 0)
				{
					_unitIdMap[unitId1]->setAttackID(unitId2);
					_unitIdMap[unitId1]->attack();
					if (_unitIdMap.count(unitId2) > 0)//if (_unitManager->_unitIdMap[_attackID]!=nullptr)
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
								_unitIdMap[unitId2]->setEnermyId(unitId1);//ؔ֯٥ܷ
								_unitIdMap[unitId2]->setUnderAttack(true);//ؔ֯٥ܷ
								_unitIdMap[unitId2]->displayHpBar();
							}
					}
					else
						_unitIdMap[unitId1]->stopAttackUpdate();
				}

			}

			else
			{
				if (receiveMessage.cmd() == MESSAGECHT)
				{
					static int timer = 0;
					if (timer++ % 8 == 0) {
						chat_out_box->setString(campString[receiveMessage.camp()] + ":" + receiveMessage.chat_message());
					}
					else {
						chat_out_box->setString(chat_out_box->getString() + "\n" + campString[receiveMessage.camp()] + ":" + receiveMessage.chat_message());
					}
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
		Unit * unit = _unitIdMap[id];
		if (unit)
		{
			if (unit->getUnitType() >= 5 && unit->getCamp() == _playerCamp)
				SimpleAudioEngine::getInstance()->playEffect("Music/Unit lost.wav");
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
					_unitIdMap[attackId]->setAutoAttack(true);//未测试
				}
			}
			if (unit->getUnitType() < 5 && unit->getEnermyId() != 0 && _unitIdMap.count(unit->getEnermyId()) > 0)//未测试
			{
				_unitIdMap[unit->getEnermyId()]->setAttackID(0);
				_unitIdMap[unit->getEnermyId()]->setAutoAttack(true);
			}

			if (unit->getCamp()==_playerCamp&&unit->getUnitType() < 5)
			{
				if (POWER[unit->getUnitType()] > 0)
					_electricity->addElectricity(-POWER[unit->getUnitType()]);
				else
					_electricity->costElectricity(POWER[unit->getUnitType()]);
			}
			if (unit->getCamp() == _playerCamp && unit->getUnitType() == BASE || unit->getUnitType() == OREREFINERY)
			{
				_money->cutMoneyInPeriod(MONEY_PRODUCE[unit->getUnitType()]);
			}
			/*if (_selectedUnitID.count(id) > 0)
				_selectedUnitID.erase(id);*/

			if (unit->getUnitType() == ATTACKDOG)
				unit->stopAllActions();
			vector<int>::iterator iter=find(_selectedUnitID.begin(),_selectedUnitID.end(),id);  
			if (iter != _selectedUnitID.end())
				_selectedUnitID.erase(iter);

			unit->removeFromMap();
			//unit->deleteUnit();
			_tileMap->removeChild(unit);
			_unitIdMap.erase(id);

			checkWinOrLose();

		}
	}
}
//BASE ,POWERPLANT,BARRACKS,WARFACTORY,OREREFINERY,GI,ATTACKDOG,TANK
Unit * UnitManager::creatUnit(CampTypes camp, UnitTypes type, const  GridVec2& pos, int id )
{
	if (id == 0)
		id = _nextId;
	Unit * unit;
	vector<string >picPath = {"units/Base(","units/PowerPlant(","units/Barrack(","units/WarFactory(","units/OreRefinery(","units/GI(","units/AttackDog(","units/Tank("};
	vector<string>picColor = { "","red","blue","green","yellow" };
	
	int transType = type;
	string unitPic = picPath[type] + picColor[camp] + ").png";
	switch (type)
	{
		case BASE:
			unit = Base::create(unitPic);
			if(camp==_playerCamp)
				setMyBaseId(id);
			break;
		case POWERPLANT:
			unit = PowerPlant::create(unitPic);
			break;
		case BARRACKS:
			if(camp==_playerCamp)
				_barracksId[id]=0;
			unit = Barracks::create(unitPic);
			break;
		case WARFACTORY:
			if (camp == _playerCamp)
				_warFactoryId[id]=0;
			unit = WarFactory::create(unitPic);
			break;
		case OREREFINERY:
			unit = OreRefinery::create(unitPic);
			break;
		case GI:
			unit = Solider::create(unitPic);
			break;
		case ATTACKDOG:
			unit = AttackDog::create(unitPic);
			break;
		case TANK:
			unit = Tank::create(unitPic);
			break;
		default:
			break;
	}
	GridVec2 correctGridPosition;
	
	if (type < 5)
	{
		correctGridPosition._x = (pos._x/32)*32 - (SIZES[type]._width / 2) * 32;
		correctGridPosition._y = (pos._y/32)*32 - (SIZES[type]._height / 2) * 32;
	}	
	else
		correctGridPosition = Vec2(pos._x,pos._y);
	
	unit->setUnitManager(this);
	unit->init( camp, type, correctGridPosition, _tileMap, _gridMap,id);
	_unitIdMap[id] = unit;
	if (type < 5)
	{
		GridVec2 realPos;
		realPos._x = SIZES[type]._width % 2 == 0 ? (pos._x/32)*32 : (pos._x/32)*32 + 16;
		realPos._y = SIZES[type]._height % 2 == 0 ? (pos._y/32)*32 : (pos._y/32)*32 + 16;
		unit->setPosition(Vec2(realPos._x,realPos._y));	
	}
	else
		unit->setPosition(Vec2(pos._x, pos._y));
	if (camp == _playerCamp)
	{
		if (type > 5)
			SimpleAudioEngine::getInstance()->playEffect("Music/Unit ready.wav");//音效
	}
	if(type<5)
		unit->buildingUpdate();
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
		if (_unitIdMap[id]->getCamp() == _playerCamp&&_unitIdMap[id]->getIsBuilding()==false)
		{
			_selectedUnitID.push_back(id);
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
	newMessage->set_unit_id1(_nextId);//鍗曚綅id鐨勮缃紵锛燂紵
	_nextId += 4;
	GridPoint * posPoint =newMessage->mutable_position();
	posPoint->set_x(  pos._x);
	posPoint->set_y( pos._y);
	//鎬庝箞璁剧疆鍧愭爣锛燂紵
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
void UnitManager::createChatMessage(std::string cM)
{
	//_textField->setText("");
	auto newMessage = _msgGroup->add_game_message();
	newMessage->set_camp(_playerCamp);
	newMessage->set_cmd(MESSAGECHT);
	newMessage->set_chat_message(cM);

}
//鏈湴绉诲姩鍜屾敾鍑荤殑娑堟伅鎬庝箞鐢熸垚锛燂紵
void UnitManager::choosePosOrUnit(const GridVec2 & pos)
{
	int idAtPos = _gridMap->getUnitIdAt(GridVec2(pos._x / 32, pos._y / 32));
	Vec2 destination = Vec2(pos._x, pos._y);
	if (!_selectedUnitID.empty())
	{
		if (idAtPos == 0 || idAtPos == _NO_PASS)
		{
			for (auto unitid : _selectedUnitID)
			{
				if (_unitIdMap[unitid]->getUnitType() >= 5)//是可移动单位
				{
					_unitIdMap[unitid]->stopAttackUpdate();
					_unitIdMap[unitid]->setAttackID(0);

					//if (destination != _unitIdMap[unitid]->getDestination())
					if (_unitIdMap[unitid]->getActionManager()->getNumberOfRunningActionsInTarget(_unitIdMap[unitid]))
					{
						_unitIdMap[unitid]->stopAllActions();
						_destinationMap.erase(GridVec2(_unitIdMap[unitid]->getDestination().x / 32, _unitIdMap[unitid]->getDestination().y / 32));

						GridVec2 unitCoord(_unitIdMap[unitid]->getPosition().x / 32, _unitIdMap[unitid]->getPosition().y / 32);
						GridRect unitRect(GridRect(GridVec2(_unitIdMap[unitid]->getPosition().x / 32, _unitIdMap[unitid]->getPosition().y / 32), GridDimen(1, 1)));
						_unitIdMap[unitid]->_battleMap->unitLeavePosition(_unitIdMap[unitid]->getUnitRect());
						_unitIdMap[unitid]->setUnitCoord(unitCoord);
						_unitIdMap[unitid]->setUnitRect(unitRect);
						_unitIdMap[unitid]->_battleMap->unitCoordStore(_unitIdMap[unitid]->getID(), unitRect);
					}

					_unitIdMap[unitid]->setDestination(destination);
					_unitIdMap[unitid]->tryToFindPath();
				}
			}
		}
		else
		{
			if (_unitIdMap[idAtPos]->getCamp() == _playerCamp
				&&_unitIdMap[idAtPos]->getIsBuilding()==false)//閫夋嫨浜嗘垜鏂圭殑鍗曚綅 鍒欐樉绀鸿鏉?
			{
				deselectAllUnits();
				_selectedUnitID.push_back(idAtPos);
				_unitIdMap[idAtPos]->displayHpBar();

			}
			else////选择了敌方单位
			{
				//1敌方建筑 到达 攻击
				//2敌方兵种 跟踪 攻击
				if (_unitIdMap[idAtPos]->getUnitType() >= 0)//if (_unitIdMap[idAtPos]->getUnitType() < 5)
				{
					if (_unitIdMap[idAtPos]->getUnitType() < 5
						&& _unitIdMap[idAtPos]->getIsBuilding() == true)
						return;
					for (auto unitid : _selectedUnitID)
					{
						if (_unitIdMap[unitid]->getUnitType() >= 5)
						{
							int distanceX = abs(getUnitPos(idAtPos)._x - getUnitPos(unitid)._x);
							int distanceY = abs(getUnitPos(idAtPos)._y - getUnitPos(unitid)._y);
							if (distanceX > AUTO_ATTACK_RANGE[_unitIdMap[unitid]->getUnitType() - 5]._width / 2
								|| distanceY > AUTO_ATTACK_RANGE[_unitIdMap[unitid]->getUnitType() - 5]._height / 2)
							{
								//移动到建筑单位附近
								_unitIdMap[unitid]->stopAttackUpdate();
								_unitIdMap[unitid]->setAttackID(idAtPos);
								if (_unitIdMap[unitid]->getActionManager()->getNumberOfRunningActionsInTarget(_unitIdMap[unitid]))
								{
									_unitIdMap[unitid]->stopAllActions();
									_destinationMap.erase(GridVec2(_unitIdMap[unitid]->getDestination().x / 32, _unitIdMap[unitid]->getDestination().y / 32));

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
							}
							else
							{
								_unitIdMap[unitid]->setAttackID(idAtPos);
								_unitIdMap[unitid]->startAttackUpdate();
								continue;
							}
							_unitIdMap[unitid]->setAttackID(idAtPos);
						}
					}
				}
				else
				{
					//2敌方兵种 跟踪 攻击
				}
			}
		}
	}
	else 
	{
		if (idAtPos != 0 && idAtPos != _NO_PASS&&
			_unitIdMap.count(idAtPos)>0&&
			_unitIdMap[idAtPos]->getCamp() == _playerCamp)//閫夋嫨浜嗘垜鏂圭殑鍗曚綅 鍒欐樉绀鸿鏉?
		{
			if (_unitIdMap[idAtPos]->getUnitType() < 5 && _unitIdMap[idAtPos]->getIsBuilding() == true)
				return;
			_selectedUnitID.push_back(idAtPos);
			_unitIdMap[idAtPos]->displayHpBar();
		}

	}
}

void UnitManager::fighterUnitProductionUpdate(float fd)
{
	auto ite = _fighterProduceSeq.begin();
	int mark = 0;
	while (ite != _fighterProduceSeq.end())
	{
		if ((*ite == GI || *ite == ATTACKDOG) && _barracksId.empty() != true)
		{
			for (auto & id : _barracksId)
			{
				if (_unitIdMap[id.first]->getIsBuilding()==false
					&&_unitIdMap[id.first]->getProducingState() == 0)
				{
					_unitIdMap[id.first]->startProduceUnit(*ite);
					/*if (*ite == GI)
						_waitingGINum--;
					else
						_waitingAttackDogNum--;*/
					ite = _fighterProduceSeq.erase(ite);
					mark = 1;
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
					if (_unitIdMap[id.first]->getIsBuilding() == false
						&&_unitIdMap[id.first]->getProducingState() == 0)
					{
						_unitIdMap[id.first]->startProduceUnit(*ite);
						ite = _fighterProduceSeq.erase(ite);
						mark = 1;
						//_waitingTankNum--;
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
	GridVec2 originPoint;
	originPoint._x = rect.origin.x / 32.0;
	originPoint._y = rect.origin.y / 32.0;
	return GridRect(originPoint,
		GridDimen(rect.size.width/32.0, rect.size.height/32.0));
}
Unit * UnitManager::getUnitPtr(int id)
{
	if (_unitIdMap.count(id) > 0)
		return _unitIdMap[id];
	else
		return nullptr;
}
Vec2 UnitManager::getMyBasePos()
{
	GridVec2 basePos;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (_myBaseId != 0&&_unitIdMap.count(_myBaseId)>0) 
	{
		basePos = _unitIdMap[_myBaseId]->getUnitCoord();
		return Vec2(basePos._x * 32, basePos._y * 32);
	}
	else 
	{
		switch (CampTypes(_socketClient->getCamp()))
		{
		case RED:
			return Point(visibleSize.width / 2, visibleSize.height / 2);
		case BLUE:
			return Point(_tileMap->getContentSize().width - visibleSize.width / 2, _tileMap->getContentSize().height - visibleSize.height / 2);
		case GREEN:
			return Point(visibleSize.width / 2, _tileMap->getContentSize().height - visibleSize.height / 2);
		case YELLOW:
			return Point(_tileMap->getContentSize().width - visibleSize.width / 2, visibleSize.height / 2);
		default:
			break;
		}
		//return Vec2(visibleSize.width / 2, visibleSize.height / 2);
	}
}
void  UnitManager::stopAllBuildingUnitUpdate() 
{
	unschedule(schedule_selector(UnitManager::fighterUnitProductionUpdate));
	for (auto barracks : _barracksId)
		_unitIdMap[barracks.first]->stopProduceUnit();
	for (auto warFactory : _warFactoryId)
		_unitIdMap[warFactory.first]->stopProduceUnit();

}
void UnitManager::startAllBuildingUnitUpdate() 
{
	//schedule(schedule_selector(UnitManager::fighterUnitProductionUpdate));
	for (auto barracks : _barracksId)
		_unitIdMap[barracks.first]->schedule(schedule_selector(BuildingUnit::produceUpdate));
	for (auto warFactory : _warFactoryId)
		_unitIdMap[warFactory.first]->schedule(schedule_selector(BuildingUnit::produceUpdate));
}
void UnitManager::checkWinOrLose()
{
	vector<int> _unitAmountInCamps;
	int totalPlayers = _socketClient->getTotalPlayer();
	_unitAmountInCamps.resize(totalPlayers + 1, 0);
	int loseCamps = 0;
	for (auto unit : _unitIdMap) 
	{
		_unitAmountInCamps[unit.second->getCamp()]++;
	}
	for (int i = RED; i <= totalPlayers; ++i) 
	{
		if (_unitAmountInCamps[i] == 0) 
		{
			_endFlag[static_cast<CampTypes>(i)] = true;
			loseCamps++;
		}
	}
	if (_endFlag[getPlayerCamp()]) 
	{
		lose();
	}
	if (!_endFlag[getPlayerCamp()] && loseCamps == _socketClient->getTotalPlayer() - 1) 
	{
		win();
	}
}

void UnitManager::win()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto win = Sprite::create("ui/end/Win.png");
	win->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	addChild(win, 300);

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
}

void UnitManager::lose()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto lose = Sprite::create("ui/end/Lose.png");
	lose->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	addChild(lose, 300);
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
}