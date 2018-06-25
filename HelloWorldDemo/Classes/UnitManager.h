
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
#include"SimpleAudioEngine.h"
#include"SocketClient.h"
#include"SocketServer.h"
#include"Electricity.h"
#include"Money.h"
#include "extensions/cocos-ext.h" 
#include "ui/CocosGUI.h"

using namespace CocosDenshion;
using namespace std;
using namespace ui;
USING_NS_CC;
USING_NS_CC_EXT;



class Unit;
class Electricity;
class UnitManager;
class GridMap;
class GridVec2;
class GridDimen;
class GridRect;
class BuildingUnit;
class FightUnit;
 
const string campString[5] = { "","RED","BLUE","GREEN","YELLOW" };

class UnitManager :public Node
{
	CC_SYNTHESIZE(TextField *, _textField, textField);
	CC_SYNTHESIZE(Text*, chat_out_box, ChatOutBox);
	CC_SYNTHESIZE(Money *, _money, money);
	CC_SYNTHESIZE(Electricity*, _electricity, electricity);
	CC_SYNTHESIZE(GameMessageGroup *, _msgGroup, MessageGrop);
	CC_SYNTHESIZE(CampTypes, _playerCamp, PlayerCamp);
	CC_SYNTHESIZE(GridMap *, _gridMap, Gridmap);
	CC_SYNTHESIZE(TMXTiledMap *, _tileMap, TileMap);
	CC_SYNTHESIZE(int, _nextId, NextId);
	CC_SYNTHESIZE(int, _waitingGINum, WaitingGINum);
	CC_SYNTHESIZE(int, _myBaseId, MyBaseId);
	CC_SYNTHESIZE(int, _waitingTankNum, WaitingTankNum);
	CC_SYNTHESIZE(int, _waitingAttackDogNum, WaitingAttackDogNum);
	vector <int> _selectedUnitID;
	map<int, int> _barracksId;
	map<int,int> _warFactoryId;
	map<int, int>newAttackUnit;
	map<int, int>attackingUnit;
	map<vector<UnitTypes>::iterator, UnitTypes> _producingFightUnit;
	map<int, vector<UnitTypes>::iterator> _buildingProducingIte;
	map<int, int> _produceTime;
	//vector<vector<UnitTypes>::iterator>_deleteFighterProduceSeqIte;
	map<CampTypes, bool> _endFlag;
public:
	vector<UnitTypes>_fighterProduceSeq;
	map<int, Unit *>_unitIdMap;
	map<GridVec2, int>_destinationMap;
	
	boost::shared_ptr<talk_to_server> _socketClient;
	socket_server * _socketServer;

	CREATE_FUNC(UnitManager);
	bool init();
	GridVec2 getUnitPos(int id);
	CampTypes getUnitCamp(int id);
	Unit * getUnitPtr(int id);
	//void produceFighter(int type ,int id);
	void updateUnitState();
	void destoryUnit(int id);
	void checkWinOrLose();
	Unit * creatUnit(CampTypes camp, UnitTypes type, const  GridVec2& pos, int id = 0);
	void creatProduceMessage(UnitTypes unitType, const GridVec2 & pos);
	void creatMoveMessage(int id, vector<GridVec2> &path);
	void createAttackMessage(int id1,int id2 ,int damage);
	void selectUnits(const GridRect &range);
	void choosePosOrUnit(const GridVec2 & pos);
	void deselectAllUnits();
	friend GridRect transferRectToGridRect(const Rect & rect);
	//void unitAttackUpdate();
	void fighterUnitProductionUpdate(float fd);
	Vec2 getMyBasePos();
	void stopAllBuildingUnitUpdate();
	void startAllBuildingUnitUpdate();
	void createChatMessage(std::string);
	//virtual void onEnter();
	//virtual void onExit();
	void win();
	void lose();
};	



#endif // !_UNITMANAGER_H




