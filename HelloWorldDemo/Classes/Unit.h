#ifndef _UNIT_H_
#define _UNIT_H_
#include "GridMap.h"
#include"cocos2d.h"
#include "ui/UILoadingBar.h"
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
const int MONEY_PRODUCE[5] = { 0,0,0,0,0 };
//每种建筑单位的SIZE
const GridDimen SIZES[5] = { GridDimen(2,2),GridDimen(0,0),
							GridDimen(0,0),GridDimen(0,0),GridDimen(0,0) };
const int FIGHTER_HEALTH[3] = {};
const GridDimen FIGHTER_SIZES[3] = { GridDimen(1,1),GridDimen(1,1),GridDimen(1,1) };
const float UNIT_MOVE_SPEED[3] = { 0,0,0 };
const int ATTACK_FORCE[3] = {0,0,0};
const int ATTACK_SPEED[3] = {0,0,0 };
const GridDimen AUTO_ATTACK_RANGE[3] = { GridDimen(),GridDimen(),GridDimen()};
const int MANUAL_ATTACK_RANGE[3] = { 0,0,0 };
const string BUILDING_BG_BAR[5] = { "","","","","" };
const string BUILDING_HP_BAR[5] = { "","","","","" };
const string FIGHTER_HP_BAR[3] = {"","",""};
const string FIGHTER_BG_BAR[3] = { "","","" };
typedef enum {
	BASE = 0,
	POWERPLANT,
	BARRACKS,
	WARFACTORY,
	OREREFINERY	
}BuildingTypes;
typedef enum {
	RED = 1,
	BLUE,
	GREEN,
	YELLOW
}CampTypes;
typedef enum
{
	GI=0,
	ATTACKDOG,
	TANK
}FightUnitType;

class Unit:public Sprite
{
	/*CC_SYNTHESIZE定义变量，并且直接定义默认的get/set方法。
	protected: varType varName;\
	public: virtual varType get##funName(void) const { return varName; }\
	public: virtual void set##funName(varType var){ varName = var; }*/
	CC_SYNTHESIZE(int, _id, ID);
	CC_SYNTHESIZE(CampTypes, _camp, Camp);
	CC_SYNTHESIZE(int, _health, Health);
	CC_SYNTHESIZE(int, _moneyCost, MoneyCost);
	CC_SYNTHESIZE(int, _currentHp, CurrentHp);
	CC_SYNTHESIZE(bool, _underAttack, UnderAttack);
	CC_SYNTHESIZE(GridDimen, _unitSize, UnitSize);
	CC_SYNTHESIZE(GridRect, _unitRect, UnitRect);
	CC_SYNTHESIZE(GridVec2, _unitCoord, UnitCoord);
public:
	LoadingBar *_hpBar=nullptr;
	Sprite * hpBGSprite=nullptr;
	//virtual bool init(int id, CampTypes camp, BuildingTypes buildingType);
	//CC_SYNTHESIZE(GridDimen, _unitView, UnitView);
	GridMap * _battleMap=nullptr;
	TMXTiledMap* _tiledMap = nullptr;
	//~Unit();
	//static Unit* Unit::create(const std::string& filename);
	//virtual void initHpBar();
	//virtual bool setPositionInGridMap(GridRect rectPos,GridMap * map)const;
	//单位收到伤害，掉血，血条缩短 当hp<=0是会自动调用unschedule凹函数
	void getDamage(int hurt);
	//将单位从和_battleMap种移除 有火焰特效
	void removeFromMap();
	
};
class BuildingUnit :public Unit
{
	CC_SYNTHESIZE(int,_powerCost,PowerCost);
	CC_SYNTHESIZE(int,_moneyProduce,MoneyProduce);
	CC_SYNTHESIZE(BuildingTypes, _buildingType, BuildingType);
public:
	//void setProperties(BuildingTypes buildingtype);
	//static BuildingUnit * create(const std::string& filename);
	//初始化时会自动调用initHpBar和setPositionInGridMap
	bool init(int id,CampTypes camp,BuildingTypes buildingType,GridVec2 point, TMXTiledMap* map,GridMap *gridmap );
	//初始化血条
	void initHpBar(BuildingTypes type);
	//在网格地图种设置该单位的信息
	bool setPositionInGridMap(GridRect rectPos, GridMap * map);
	//从地图中移除单位
	void deleteUnit();
	
	//virtual void startProduce();
	//bool setPositionInTiledMap();
	//virtual void sellBuilding();
};
class FightUnit :public Unit
{
	CC_SYNTHESIZE(FightUnitType, _fighterType, FighterType);
	CC_SYNTHESIZE(bool,_attacking,Attacking);
	CC_SYNTHESIZE(float,_moveSpeed,MoveSpeed);
	CC_SYNTHESIZE(int ,_attackForce,AttackForce);
	CC_SYNTHESIZE(int ,_attackSpeed,AttackSpeed);
	CC_SYNTHESIZE(int ,_attackID,AttackID);
	CC_SYNTHESIZE(int ,_atkIDPosition,AtkIDPosition);
	CC_SYNTHESIZE(GridVec2 ,_destination,Destination);
	CC_SYNTHESIZE(int ,_manualAttackScope,ManualAttackScope);
	CC_SYNTHESIZE(GridDimen ,_autoAttackScope,AutoAttackScope);
public:
	bool init(int id,GridVec2 coord,CampTypes camp,FightUnitType type, 
		TMXTiledMap* map, GridMap * gridmap);
	//bool setPositionInTiledMap();
	bool setPositionInGirdMap(GridRect rectPos, int id);
	void move();
	virtual void attack();
	virtual void autoAttack();
	void searchNearEnemy();
	virtual bool setPosition();
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
#endif // !_UNIT_H
