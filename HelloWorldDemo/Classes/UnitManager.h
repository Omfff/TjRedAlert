#ifndef _UNITMANAGER_H_
#define _UNITMANAGER_H_

#include<cocos2d.h>
#include"GridMap.h"
#include"Unit.h"
#include<map>
#include<vector>
#include"GridMap.h"
#include"GameMessages.pb.h"
#include"SystemHeader.h"
using namespace std;
USING_NS_CC;
class Unit;
class UnitManager;
//class BuildingUnit;
class UnitManager :public Node
{
	CC_SYNTHESIZE(GameMessageGroup *, _msgGroup, MessageGrop);
	CC_SYNTHESIZE(CampTypes, _playerCamp, PlayerCamp);
	CC_SYNTHESIZE(GridMap *, _gridMap, Gridmap);
	CC_SYNTHESIZE(TMXTiledMap *, _tileMap, TileMap);
	CC_SYNTHESIZE(int, _nextId, NextId);
	map<int, Unit *>_unitIdMap;
	vector<int> _selectedUnitID;

	//Base * base = nullptr;
public:
	CREATE_FUNC(UnitManager);
	bool init();
	GridVec2 getUnitPos(int id);
	CampTypes getUnitCamp(int id);
	//void produceFighter(int type ,int id);
	void updateUnitState();
	void destoryUnit(int id);
	void checkWinOrLose();
	Unit * creatUnit(CampTypes camp, UnitTypes type, const  GridVec2& pos, int id = 0);
	void creatProduceMessage(UnitTypes unitType, const GridVec2 & pos);
	void creatMoveMessage(int id, const Vec2 & pos);
	void selectUnits(const GridRect &range);
	void choosePosOrUnit(const GridVec2 & pos);
	void deselectAllUnits();
	friend GridRect transferRectToGridRect(const Rect & rect);
	
};



#endif // !_UNITMANAGER_H


