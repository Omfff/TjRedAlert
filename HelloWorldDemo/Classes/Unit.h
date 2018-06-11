#ifndef _UNIT_H_
#define _UNIT_H_
#include "GridMap.h"
#include<cocos2d.h>
#include "ui/UILoadingBar.h"
#include "UnitManager.h"
#include"PathFinder.h"
#include"SystemHeader.h"




USING_NS_CC;
using namespace ui;
//默认顺序：按BASE ,POWERPLANT,BARRACKS,WARFACTORY,OREREFINERY
//每种建筑单位的价格（默认顺序）
const int COST[5] = { 2500,800,500,2000,2000 };
//每种建筑单位的HP（默认顺序）
const int HEALTH[5] = { 2000,750,900,1200,1000 };
//每种建筑单位的发电/耗电量（默认顺序）
const int POWER[5] = { 50,200,-10,-50,-40 };
//每种建筑单位的金钱产量（默认顺序）
const int MONEY_PRODUCE[5] = {50,0,0,0,200};
//每种建筑单位的SIZE
const GridDimen SIZES[5] = { GridDimen(3,2),GridDimen(2,2),
GridDimen(2,2),GridDimen(3,2),GridDimen(3,2) };
const int FIGHTER_HEALTH[3] = {400,400,400};
const GridDimen FIGHTER_SIZES[3] = { GridDimen(1,1),GridDimen(1,1),GridDimen(1,1) };
const float UNIT_MOVE_SPEED[3] = { 3,5,6 };
const int ATTACK_FORCE[3] = { 50,50,30 };
const int ATTACK_SPEED[3] = { 0,0,0 };
const GridDimen AUTO_ATTACK_RANGE[3] = { GridDimen(),GridDimen(),GridDimen() };
const int MANUAL_ATTACK_RANGE[3] = { 0,0,0 };
const string BUILDING_BG_BAR[5] = { "20150914105931941.png","20150914105931941.png","20150914105931941.png","20150914105931941.png","20150914105931941.png" };
const string BUILDING_HP_BAR[5] = { "20150914105957633.png","20150914105957633.png","20150914105957633.png","20150914105957633.png","20150914105957633.png" };
const string FIGHTER_HP_BAR[3] = { "20150914105957633.png","20150914105957633.png","20150914105957633.png" };
const string FIGHTER_BG_BAR[3] = { "20150914105931941.png","20150914105931941.png","20150914105931941.png" };


class UnitManager;
class Unit:public Sprite
{
	/*CC_SYNTHESIZE定义变量，并且直接定义默认的get/set方法。
	protected: varType varName;\
*/
	CC_SYNTHESIZE(UnitTypes, _unitType, UnitType);
	CC_SYNTHESIZE(int, _id, ID);
	CC_SYNTHESIZE(CampTypes, _camp, Camp);
	CC_SYNTHESIZE(int, _health, Health);
	CC_SYNTHESIZE(int, _moneyCost, MoneyCost);
	CC_SYNTHESIZE(int, _currentHp, CurrentHp);
	CC_SYNTHESIZE(bool, _underAttack, UnderAttack);
	CC_SYNTHESIZE(GridDimen, _unitSize, UnitSize);
	CC_SYNTHESIZE(GridRect, _unitRect, UnitRect);
	CC_SYNTHESIZE(GridVec2, _unitCoord, UnitCoord);
	CC_SYNTHESIZE(Vec2, _destination, Destination);
	CC_SYNTHESIZE(int, _attackID, AttackID);
	//CC_SYNTHESIZE(UnitManager*，_unitManager, unitManager);
public:
	UnitManager * _unitManager = nullptr;
	LoadingBar *_hpBar=nullptr;
	Sprite * hpBGSprite=nullptr;
	std::vector<GridVec2> _gridPath;
	void setUnitManager(UnitManager * uM) { _unitManager = uM; }
	virtual bool init(CampTypes camp,UnitTypes Type,GridVec2 point, TMXTiledMap* map,GridMap *gridmap , int id=0);
	//CC_SYNTHESIZE(GridDimen, _unitView, UnitView);
	GridMap * _battleMap=nullptr;
	TMXTiledMap* _tiledMap = nullptr;
	//~Unit();
	static Unit* Unit::create(const std::string& filename);
	//virtual void initHpBar();
	//virtual bool setPositionInGridMap(GridRect rectPos,GridMap * map)const;
	//单位收到伤害，掉血，血条缩短 当hp<=0是会自动调用unschedule凹函数
	bool getDamage(int hurt);
	//将单位从和_battleMap种移除 有火焰特效
	void removeFromMap();
	void hideHpBar();
	void displayHpBar();
	virtual void deleteUnit() {}

	//brief try to find path
	virtual void tryToFindPath() {};

	//find a path to the destination
	virtual std::vector<GridVec2> findPath(const GridVec2 & destination) const { return _gridPath; };

	//optimize path 
	virtual void optimizePath() {};

	virtual void move();
	virtual void attack() {}
	//初始化血条
	void initHpBar(UnitTypes type);
	virtual void attackUpdate(float dt) {}
	virtual void startAttackUpdate() {}
	virtual void stopAttackUpdate() {}
	virtual GridVec2 findEmptyPosToProduceSolider() { return GridVec2(0, 0); }
	virtual GridVec2 findEmptyPosToProduceTank() { return GridVec2(0, 0); }
	
};
class BuildingUnit :public Unit
{
	CC_SYNTHESIZE(int,_powerCost,PowerCost);
	CC_SYNTHESIZE(int,_moneyProduce,MoneyProduce);
	CC_SYNTHESIZE(UnitTypes, _buildingType, BuildingType);
public:
	//void setProperties(UnitTypes buildingtype);
	//static BuildingUnit * create(const std::string& filename);
	//初始化时会自动调用initHpBar和setPositionInGridMap
	virtual bool init(CampTypes camp,UnitTypes buildingType,GridVec2 point, TMXTiledMap* map,GridMap *gridmap, int id=0 );
	
	//在网格地图种设置该单位的信息
	bool setPositionInGridMap(GridRect rectPos, GridMap * map);
	//从地图中移除单位
	virtual void deleteUnit();
	
	//virtual void startProduce();
	//bool setPositionInTiledMap();
	//virtual void sellBuilding();
};
class FightUnit :public Unit
{
	CC_SYNTHESIZE(UnitTypes, _fighterType, FighterType);
	CC_SYNTHESIZE(bool,_attacking,Attacking);
	CC_SYNTHESIZE(float,_moveSpeed,MoveSpeed);
	CC_SYNTHESIZE(int ,_attackForce,AttackForce);
	CC_SYNTHESIZE(int ,_attackSpeed,AttackSpeed);
	CC_SYNTHESIZE(bool, _autoAttack, AutoAttack);
	CC_SYNTHESIZE(int ,_atkIDPosition,AtkIDPosition);
	
	CC_SYNTHESIZE(int ,_manualAttackScope,ManualAttackScope);
	CC_SYNTHESIZE(GridDimen ,_autoAttackScope,AutoAttackScope);
public:
	virtual bool init(CampTypes camp,UnitTypes type, GridVec2 coord,
		TMXTiledMap* map, GridMap * gridmap,int id );
	//bool setPositionInTiledMap();
	bool setPositionInGirdMap(GridRect rectPos, int id);
	
	//brief try to find path
	void tryToFindPath();

	//optimize path 
	void optimizePath();

	//find a path to the destination
	std::vector<GridVec2> findPath(const GridVec2 & destination);

	void move();
	virtual void shootBullet();
	virtual void attack();
	//void initHpBar(UnitTypes type) {}
	bool setPositionInGridMap(GridRect rectPos, GridMap * map) {}
	//virtual void autoAttack();
	void searchNearEnemy();
	//virtual bool setPosition();
};
//爆炸特效类
class ExplosionEffect : public cocos2d::ParticleFire
{
public:
	 bool init();

	CREATE_FUNC(ExplosionEffect);
private:
	void remove(float f);
};
#endif // !_UNIT_H_
