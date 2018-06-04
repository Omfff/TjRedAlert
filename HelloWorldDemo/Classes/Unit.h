#ifndef _UNIT_H_
#define _UNIT_H_
#include "GridMap.h"
#include"cocos2d.h"
#include "ui/UILoadingBar.h"
USING_NS_CC;
using namespace ui;
//Ĭ��˳�򣺰�BASE ,POWERPLANT,BARRACKS,WARFACTORY,OREREFINERY
//ÿ�ֽ�����λ�ļ۸�Ĭ��˳��
const int COST[5] = { 2500,800,500,2000,2000 };
//ÿ�ֽ�����λ��HP��Ĭ��˳��
const int HEALTH[5] = { 2000,750,900,1200,1000 };
//ÿ�ֽ�����λ�ķ���/�ĵ�����Ĭ��˳��
const int POWER[5] = { 50,200,-10,-50,-40 };
//ÿ�ֽ�����λ�Ľ�Ǯ������Ĭ��˳��
const int MONEY_PRODUCE[5] = { 0,0,0,0,0 };
//ÿ�ֽ�����λ��SIZE
const GridDimen SIZES[5] = { GridDimen(2,2),GridDimen(0,0),
							GridDimen(0,0),GridDimen(0,0),GridDimen(0,0) };
typedef enum {
	BASE = 100,
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
	GI=1,
	ATTACKDOG,
	TANK
}FightUnitType;

class Unit:public Sprite
{
	/*CC_SYNTHESIZE�������������ֱ�Ӷ���Ĭ�ϵ�get/set������
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
	CC_SYNTHESIZE(GridRect, _unitRect, UNitRect);
	CC_SYNTHESIZE(GridVec2, _unitCoord, UnitCoord);
public:
	LoadingBar *_hpBar=nullptr;
	Sprite * hpBGSprite=nullptr;
	//virtual bool init(int id, CampTypes camp, BuildingTypes buildingType);
	//CC_SYNTHESIZE(GridDimen, _unitView, UnitView);
	GridMap * _battleMap=nullptr;
	TMXTiledMap* _tiledMap = nullptr;
	//~Unit();
	static Unit* Unit::create(const std::string& filename);
	//virtual void initHpBar();
	//virtual bool setPositionInGridMap(GridRect rectPos,GridMap * map)const;
	//��λ�յ��˺�����Ѫ��Ѫ������ ��hp<=0�ǻ��Զ�����unschedule������
	void getDamage(int hurt);
	//����λ�Ӻ�_battleMap���Ƴ� �л�����Ч
	void removeFromMap();
	
};
class BuildingUnit :public Unit
{
	CC_SYNTHESIZE(int,_powerCost,PowerCost);
	CC_SYNTHESIZE(int,_moneyProduce,MoneyProduce);
	CC_SYNTHESIZE(BuildingTypes, _buildingType, BuildingType);
public:
	//void setProperties(BuildingTypes buildingtype);
	static BuildingUnit * create(const std::string& filename);
	//��ʼ��ʱ���Զ�����initHpBar��setPositionInGridMap
	bool init(int id,CampTypes camp,BuildingTypes buildingType,GridVec2 point, TMXTiledMap* map,GridMap *gridmap );
	//��ʼ��Ѫ��
	void initHpBar();
	//�������ͼ�����øõ�λ����Ϣ
	bool setPositionInGridMap(GridRect rectPos, GridMap * map);
	//�ӵ�ͼ���Ƴ���λ
	void deleteUnit();
	
	//virtual void startProduce();
	//bool setPositionInTiledMap();
	//virtual void sellBuilding();
};
class FightUnit :public Unit
{
	CC_SYNTHESIZE(bool,_attacking,Attacking);
	CC_SYNTHESIZE(int,_moveSpeed,MoveSpeed);
	CC_SYNTHESIZE(int ,_attackForce,AttackForce);
	CC_SYNTHESIZE(int ,_attackSpeed,AttackSpeed);
	CC_SYNTHESIZE(int ,_attackID,AttackID);
	CC_SYNTHESIZE(int ,_atkIDPosition,AtkIDPosition);
	CC_SYNTHESIZE(GridVec2 ,_destination,Destination);
	CC_SYNTHESIZE(GridDimen ,_manualAttackScope,ManualAttackScope);
	CC_SYNTHESIZE(GridDimen ,_autoAttackScope,AutoAttackScope);
public:
	bool init(int id,int camp,FightUnitType type);
	bool setPositionInTiledMap();
	void move();
	virtual void attack();
	virtual void autoAttack();
	void searchNearEnemy();
	virtual bool setPosition();
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
#endif // !_UNIT_H
