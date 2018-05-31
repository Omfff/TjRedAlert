#include"UnitManager.h"
#include"GameMessages.pb.h"
#include"Buliding.h"
#include"Unit.h"
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
				_unitIdMap[id] = newUnit;
				newUnit->setPosition(Vec2(receiveMessage.position().x(), receiveMessage.position().y()));
				_tileMap->addChild(newUnit, 10);
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
		//unit->removeFromMap();
		//unit->deleteUnit();
		_unitIdMap.erase(id);	
	}
}
//BASE ,POWERPLANT,BARRACKS,WARFACTORY,OREREFINERY,GI,ATTACKDOG,TANK
Unit * UnitManager::creatUnit(CampTypes camp, UnitTypes type, const  GridVec2& pos, int id )
{
	if (id == 0)
		id = _nextId;
	Unit * unit;
	vector<string >picPath = {"base_","","","","","","",""};
	int transType = type;
	string unitPic = picPath[type] + to_string(transType) + ".png";
	switch (type)
	{
		case BASE:
			unit = Base::create(unitPic);
			break;
		case POWERPLANT:
			break;
		case BARRACKS:
			break;
		case WARFACTORY:
			break;
		case OREREFINERY:
			break;
		case GI:
			break;
		case ATTACKDOG:
			break;
		case TANK:
			break;
		default:
			break;
	}
	unit->init( camp, type, pos, _tileMap, _gridMap,id);
	_unitIdMap[id] = unit;
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
//本地移动和攻击的消息怎么生成？？
void UnitManager::choosePosOrUnit(const GridVec2 & pos)
{
	if (!_selectedUnitID.empty())
	{
		if (_gridMap->getUnitIdAt(GridVec2(pos._x / 32, pos._y / 32)) == 0||
			_gridMap->getUnitIdAt(GridVec2(pos._x / 32, pos._y / 32)) ==_NO_PASS)
		{
			for (auto unitid : _selectedUnitID)
			{
				if (1)//_unitIdMap[unitid]->getUnitType()>=5//是可移动单位
					_unitIdMap[unitid]->move();	
			}
		}
		else
		{
			if (1)//选择了我方的单位 则显示血条
			{

			}
			else////选择了敌方单位
			{
				//1敌方建筑 到达 攻击
				//2敌方兵种 跟踪 攻击
			}
		}
	}
}
GridRect transferRectToGridRect(const Rect & rect)
{
	return GridRect(GridVec2((float)(rect.origin.x) / 32.0, (float)(rect.origin.y / 32.0)),
		GridDimen(rect.size.width/32.0, rect.size.height/32.0));
}