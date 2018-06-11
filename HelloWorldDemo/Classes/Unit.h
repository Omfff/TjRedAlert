#ifndef _UNIT_H_
#define _UNIT_H_

#include "GridMap.h"
#include "cocos2d.h"
#include "ui/UILoadingBar.h"
#include "UnitManager.h"
#include"SystemHeader.h"

USING_NS_CC;
using namespace ui;

//Ĭ��˳�򣺰�BASE ,POWERPLANT,BARRACKS,WARFACTORY,OREREFINERY
//ÿ�ֽ�����λ�ļ۸�Ĭ��˳��
const int COST[8] = { 2500,800,500,2000,2000 ,150,150,150};
//ÿ�ֽ�����λ��HP��Ĭ��˳��
const int HEALTH[5] = { 2000,750,900,1200,1000 };
//ÿ�ֽ�����λ�ķ���/�ĵ�����Ĭ��˳��
const int POWER[5] = { 50,200,-10,-50,-40 };
//ÿ�ֽ�����λ�Ľ�Ǯ������Ĭ��˳��
const int MONEY_PRODUCE[5] = {50,0,0,0,200};
//ÿ�ֽ�����λ��SIZE
const GridDimen SIZES[5] = { GridDimen(3,2),GridDimen(2,2),
GridDimen(2,2),GridDimen(3,2),GridDimen(3,2) };
const int FIGHTER_HEALTH[3] = {400,400,400};
const GridDimen FIGHTER_SIZES[3] = { GridDimen(1,1),GridDimen(1,1),GridDimen(1,1) };
const float UNIT_MOVE_SPEED[3] = { 3,5,6 };
const int ATTACK_FORCE[3] = { 50,50,30 };
const int ATTACK_SPEED[3] = { 0,0,0 };
const GridDimen AUTO_ATTACK_RANGE[3] = { GridDimen(10,10),GridDimen(9,9),GridDimen(9,9) };
const int MANUAL_ATTACK_RANGE[3] = { 0,0,0 };
const string BUILDING_BG_BAR[5] = { "20150914105931941.png","20150914105931941.png","20150914105931941.png","20150914105931941.png","20150914105931941.png" };
const string BUILDING_HP_BAR[5] = { "20150914105957633.png","20150914105957633.png","20150914105957633.png","20150914105957633.png","20150914105957633.png" };
const string FIGHTER_HP_BAR[3] = { "20150914105957633.png","20150914105957633.png","20150914105957633.png" };
const string FIGHTER_BG_BAR[3] = { "20150914105931941.png","20150914105931941.png","20150914105931941.png" };
const int PRODUCEGITIME = 120;
const int PRODUCEATTACKDOGTIME = 120;
const int PRODUCETANKTIME = 180;

class UnitManager;
class Unit:public Sprite
{
	/*CC_SYNTHESIZE�������������ֱ�Ӷ���Ĭ�ϵ�get/set������
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
	CC_SYNTHESIZE(UnitTypes, _producingUnitType, ProducingUnitType);
	CC_SYNTHESIZE(int, _producingState, ProducingState);
	CC_SYNTHESIZE(int, _enermyId, EnermyId);
	//CC_SYNTHESIZE(UnitManager*��_unitManager, unitManager);
public:
	UnitManager * _unitManager = nullptr;
	LoadingBar *_hpBar=nullptr;
	Sprite * hpBGSprite=nullptr;
	void setUnitManager(UnitManager * uM) { _unitManager = uM; }
	virtual bool init(CampTypes camp,UnitTypes Type,GridVec2 point, TMXTiledMap* map,GridMap *gridmap , int id=0);
	//CC_SYNTHESIZE(GridDimen, _unitView, UnitView);
	GridMap * _battleMap=nullptr;
	TMXTiledMap* _tiledMap = nullptr;
	//~Unit();
	static Unit* Unit::create(const std::string& filename);
	//virtual void initHpBar();
	//virtual bool setPositionInGridMap(GridRect rectPos,GridMap * map)const;
	//��λ�յ��˺�����Ѫ��Ѫ������ ��hp<=0�ǻ��Զ�����unschedule������
	bool getDamage(int hurt);
	//����λ�Ӻ�_battleMap���Ƴ� �л�����Ч
	void removeFromMap();
	void hideHpBar();
	void displayHpBar();
	virtual void deleteUnit() {}
	virtual void move();
	virtual void attack() {}
	//��ʼ��Ѫ��
	void initHpBar(UnitTypes type);
	virtual void attackUpdate(float dt) {}
	virtual void startAttackUpdate() {}
	virtual void stopAttackUpdate() {}
	virtual GridVec2 findEmptyPosToProduce() { return GridVec2(0, 0); }
	virtual GridVec2 findEmptyPosToProduceTank() { return GridVec2(0, 0); }
	virtual void startProduceUnit(UnitTypes proUnitType) {}
	virtual void stopProduceUnit() {}
};


class BuildingUnit :public Unit
{
	int _produceProcess;
	int _produceTime;
	CC_SYNTHESIZE(int,_powerCost,PowerCost);
	CC_SYNTHESIZE(int,_moneyProduce,MoneyProduce);
	CC_SYNTHESIZE(UnitTypes, _buildingType, BuildingType);
public:
	//void setProperties(UnitTypes buildingtype);
	//static BuildingUnit * create(const std::string& filename);
	//��ʼ��ʱ���Զ�����initHpBar��setPositionInGridMap
	virtual bool init(CampTypes camp,UnitTypes buildingType,GridVec2 point, TMXTiledMap* map,GridMap *gridmap, int id=0 );
	
	//�������ͼ�����øõ�λ����Ϣ
	bool setPositionInGridMap(GridRect rectPos, GridMap * map);
	//�ӵ�ͼ���Ƴ���λ
	virtual void deleteUnit();
	void startProduceUnit(UnitTypes proUnitType);
	void stopProduceUnit();
	void produceUpdate(float fd);
	
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
		TMXTiledMap* map, GridMap * gridmap,int id=0 );
	//bool setPositionInTiledMap();
	bool setPositionInGirdMap(GridRect rectPos, int id);
	void move();
	virtual void shootBullet();
	virtual void attack();
	//void initHpBar(UnitTypes type) {}
	bool setPositionInGridMap(GridRect rectPos, GridMap * map) ;
	//virtual void autoAttack();
	bool searchNearEnemy();
	//virtual bool setPosition();
	
	//void autoAttackUpdate();
};
//��ը��Ч��
class ExplosionEffect : public cocos2d::ParticleFire
{
public:
	 bool init();

	CREATE_FUNC(ExplosionEffect);
private:
	void remove(float f);
};
#endif // !_UNIT_H_
