
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
				if (_unitIdMap.count(id))
				{
					Unit * unit = _unitIdMap[id];
					unit->setDestination(Vec2(receiveMessage.position().x()*32.0,receiveMessage.position().y()*32.0));//这个position的参数都是int的
					unit->move();
				}
				else
				{
					log("empty _unitIdMap doesn't contain receiveMessage.unit_id1 ,the cmd mov is wrong !");
				}
			}
			else
			{
				if (receiveMessage.cmd() == MESSAGEATK)//unit1收到unit2攻击
				{
					int unitId1 = receiveMessage.unit_id1();
					int unitId2 = receiveMessage.unit_id2();
					int damage = receiveMessage.damage();
					Unit * unit = _unitIdMap[unitId1];
					if (receiveMessage.camp() == _playerCamp)
					{
						if (!unit->getDamage(damage))
						{
							destoryUnit(unitId1);
						}
						else
						{

						}
					}
				}
			}
		}
	}
	_msgGroup->clear_game_message();
}
void UnitManager::destoryUnit(int id)
{
	Unit * unit = _unitIdMap[id];
	if (unit)
	{
		if (unit->getUnitType() == WARFACTORY)
			_warFactoryId.erase(unit->getID());
		if (unit->getUnitType() == BARRACKS)
			_barracksId.erase(unit->getID());
		unit->removeFromMap();
		//unit->deleteUnit();
		_tileMap->removeChild(unit);
		_unitIdMap.erase(id);	
		
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
			setMyBaseId(id);
			break;
		case POWERPLANT:
			unit = PowerPlant::create(unitPic);
			break;
		case BARRACKS:
			_barracksId[id]=0;
			unit = Barracks::create(unitPic);
			break;
		case WARFACTORY:
			_warFactoryId[id]=0;
			unit = WarFactory::create(unitPic);
			break;
		case OREREFINERY:
			unit = OreRefinery::create(unitPic);
			break;
		case GI:
			unit = Solider::create("solider.png");
			break;
		case ATTACKDOG:
			break;
		case TANK:
			unit = Tank::create("footman_front_0.png");
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
	Unit *unit = _unitIdMap[id];
	if (unit)
		return unit->getUnitCoord();
	else
		return GridVec2(-1, -1);
}
CampTypes UnitManager::getUnitCamp(int id)
{
	Unit * unit = _unitIdMap[id];
	if (unit)
		return (unit->getCamp());
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
	newMessage->set_unit_id1(_nextId);//单位id的设置？？？
	_nextId += 4;
	GridPoint * posPoint =newMessage->mutable_position();
	posPoint->set_x(  pos._x);
	posPoint->set_y( pos._y);
	//怎么设置坐标？？
}
void UnitManager::creatMoveMessage(int id, const Vec2 & pos)
{
	auto newMessage = _msgGroup->add_game_message();
	newMessage->set_camp(_playerCamp);
	newMessage->set_cmd(MESSAGEMOV);
	newMessage->set_unit_id1(id);
	GridPoint * posPoint = newMessage->mutable_position();
	posPoint->set_x(pos.x);
	posPoint->set_y(pos.y);
}
void UnitManager::createAttackMessage(int id1, int id2, int damage)
{
	auto newMessage = _msgGroup->add_game_message();
	//newMessage->set_camp(_playerCamp);
	newMessage->set_cmd(MESSAGEATK);
	newMessage->set_unit_id1(id1);
	newMessage->set_unit_id2(id2);
	newMessage->set_damage(damage);
}
//本地移动和攻击的消息怎么生成？？
void UnitManager::choosePosOrUnit(const GridVec2 & pos)
{
	int idAtPos = _gridMap->getUnitIdAt(GridVec2(pos._x / 32, pos._y / 32));
	if (!_selectedUnitID.empty())
	{
		if (idAtPos == 0||idAtPos ==_NO_PASS)
		{
			for (auto unitid : _selectedUnitID)
			{
				if (_unitIdMap[unitid]->getUnitType()>=5)//是可移动单位
				{
					//寻路
					//产生移动消息
					if (attackingUnit.count(unitid) > 0)
					{
						_unitIdMap[unitid]->stopAttackUpdate();
						attackingUnit.erase(unitid);
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
			if (_unitIdMap[idAtPos]->getCamp()==_playerCamp)//选择了我方的单位 则显示血条
			{
				/*deselectAllUnits();
				_selectedUnitID.push_back(idAtPos);
				_unitIdMap[idAtPos]->displayHpBar();*/
				for (auto unitid : _selectedUnitID)
				{
					if (_unitIdMap[unitid]->getUnitType() >= 5)//是可攻击单位
					{
						if (_unitIdMap[idAtPos] != nullptr)
						{
							newAttackUnit[unitid] = idAtPos;

							/*_unitIdMap[unitid]->setAttackID(idAtPos);
							_unitIdMap[unitid]->attack();
							_unitIdMap[idAtPos]->getDamage(100);
							_unitIdMap[idAtPos]->displayHpBar();
							if (_unitIdMap[idAtPos]->getCurrentHp() <= 0)
							{
								destoryUnit(idAtPos);
							}*/
						}
						
					}
				}
			}
			else////选择了敌方单位
			{
				//1敌方建筑 到达 攻击
				//2敌方兵种 跟踪 攻击
				/*for (auto unitid : _selectedUnitID)
				{
					if (_unitIdMap[unitid]->getUnitType() >= 5)//是可攻击单位
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
			}
		}
	}
}
void UnitManager::unitAttackUpdate()
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
			_unitIdMap[battleUnit.second]->setEnermyId(battleUnit.first);//测试自动攻击
			_unitIdMap[battleUnit.second]->setUnderAttack(true);
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
				if (_unitIdMap[ite->first] == nullptr)
				{
					ite = attackingUnit.erase(ite);
				}
				else
				{
					_unitIdMap[ite->first]->stopAttackUpdate();
					//this->unschedule(schedule_selector(_unitIdMap[battleUnit.first]->attackUpdate));
					_unitIdMap[ite->first]->setAttackID(0);
					destoryUnit(ite->second);
					ite = attackingUnit.erase(ite);
				}
				
				//deleUnitId.push_back(ite->first);
			}
			else if (_unitIdMap[ite->first] == nullptr)
			{
				ite = attackingUnit.erase(ite);
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
}
void UnitManager::fighterUnitProductionUpdate()
{
	auto ite = _fighterProduceSeq.begin();
	int mark = 0;
	while (ite != _fighterProduceSeq.end())
	{
		if ((*ite == GI || *ite == ATTACKDOG) && _barracksId.empty() != true)
		{
			for (auto & id : _barracksId)
			{
				if (_unitIdMap[id.first]->getProducingState() == 0)
				{
					_unitIdMap[id.first]->startProduceUnit(*ite);
					if (*ite == GI)
						_waitingGINum--;
					else
						_waitingAttackDogNum--;
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
					if (_unitIdMap[id.first]->getProducingState() == 0)
					{
						_unitIdMap[id.first]->startProduceUnit(*ite);
						ite = _fighterProduceSeq.erase(ite);
						mark = 1;
						_waitingTankNum--;
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
	return _unitIdMap[id];
}
Vec2 UnitManager::getMyBasePos()
{
	GridVec2 basePos;
	if (_myBaseId != 0)
		basePos = _unitIdMap[_myBaseId]->getUnitCoord();
	return Vec2(basePos._x * 32, basePos._y * 32);
}