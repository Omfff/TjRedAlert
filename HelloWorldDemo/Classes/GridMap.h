#ifndef _Grid_H_
#define _Grid_H_

#include"cocos2d.h"
#include"SystemHeader.h"
#include<map>
#include<vector>
#include<set>
//#include"unitManager.h"
USING_NS_CC;
#define _MAP_WIDTH 125
#define _MAP_HEIGHT 62
#define _NO_PASS 10086
using namespace std;


//class UnitManager;

//��cocos�Դ���vec2��float��תΪ����Ϊint������ֵ
//�������˼Ӽ��˵Ⱥ������
class GridVec2
{
public:
	int _x;
	int _y;
	GridVec2():_x(0),_y(0){}
	GridVec2(const int x, const int y) :_x(x), _y(y) {}
	GridVec2(float x, float y):_x(x), _y(y) {}
	GridVec2(const Vec2 &point);
	friend GridVec2 operator +(const GridVec2 & point1,const GridVec2 &point2);
	friend GridVec2 operator -(const GridVec2 & point1, const GridVec2 &point2);
	friend bool operator ==(const GridVec2 & point1, const GridVec2 & point2);
	friend bool operator < (const GridVec2 & point1, const GridVec2 & point2);
};
//��cocos�Դ���size����ת��Ϊint��dimen
//������==�����
class GridDimen
{
public:
	int _width;
	int _height;
	GridDimen() :_width(0), _height(0) {}
	GridDimen(float width, float height) :_width(width), _height(height) {}
	GridDimen(Size s) :_width(s.width), _height(s.height) {};
	friend bool operator == (const GridDimen & size1, const GridDimen & size2);
};
//��cocos�Դ���rect����תΪ��������Ϊint��
//�����ֳ�ʼ���ķ�ʽ
class GridRect
{
public:
	GridVec2 _oriPoint;
	GridDimen _dimen;
	//���ֳ�ʼ����ʽ
	GridRect() {}
	GridRect(float x, float y, float width, float height) :_oriPoint(x, y), _dimen(width, height) {}
	GridRect(const GridVec2 & point, const GridDimen & size) :_oriPoint(point), _dimen(size) {}
	GridRect(const Rect & rect) :_oriPoint(rect.origin), _dimen(rect.size) {};
	//������==�����
	friend bool operator ==(const GridRect & rect1,const GridRect &rect2);
	//point�Ƿ��ڸ�GridRect�ڣ����ڷ���true�����򷵻�false
	bool containsPoint(const GridVec2 & point)const;
	//rect�Ƿ����GridRect�ཻ
	bool intersectsRect(const GridRect & rect)const;
	//rect�Ƿ��ڸ�GridRect��
	bool insideRect(const GridRect &rect)const;
};
//�����ͼ
class GridMap:public Layer
{
protected:
	//UnitManager * _unitManager;
	map<int, GridVec2> _unitCoord;
	//�������λ����������
	vector <vector<int>> _barrierMap;
	//�洢�����ͼ��ÿ�����ӵ�״̬ 0Ϊ�� ����Ĵ���unit��id���ߵ�ͼ�ϰ�����
	
	GridRect _worldRect;
	//��0��0�� width=125 height=62
	GridRect _ladderRect1;
	GridRect _ladderRect2;
	GridRect _ladderRect3;
	GridRect _ladderRect4;
public:

	vector <vector<int>> _findPathMap;
	CREATE_FUNC(GridMap);
	GridMap();
	//��ʼ��
	bool unitCoordStore(int unitId, const GridVec2 &position);
	bool unitCoordStore(int unitId, const GridRect & rect);
	//�洢��λ��λ����Ϣ ����trueΪ�ɹ�
	void unitPositionOccupy(int unitId, const GridVec2& position);
	void unitPositionOccupy(int unitId, const GridRect & rect);
	//�ڴ洢��λ��λ����Ϣ����Զ����õ�λռ�������ͼ�ϵ�����
	void unitLeavePosition(int unitId, const GridVec2& posioton);
	void unitLeavePosition(const GridRect & rect);
	//���Ƴ���λ��λ����Ϣ����Զ����õ�λռ����������
	//���Զ����û�ûд����������
	void unitCoordRemove(int unitId,GridRect unitRect);
	//�Ƴ���λ��Ϣ��һ�����ڵ�λ����ʱ���ã�
	int getUnitIdAt(const GridVec2& position)const;
	//��ȡpositionλ���ϵĵ�λ��Ϣ
	set <int> getUnitIdAt(const GridRect & range)const;
	//��ȡһ����Χ�ڵ����е�λid
	bool checkPointPosition(const GridVec2 & point)const;
	//�ж�point�������ͼ���Ƿ�Ϊ�� ����Ϊ���򷵻�true����������Ϊ���򷵻�false 
	//����point������ͼ��Ҳ����true��������
	bool checkRectPosition(const GridRect & rect)const;
	//�ж�rect�������ͼ���Ƿ�Ϊ�� ����Ϊ���򷵻�true����������rect���ڵ�ͼ����Ҳ����true��
	GridVec2 getEmptyPointNearby(const GridVec2 & position)const;
	//��ȡpoint�����յ����������
	GridRect getEmptyRectNearby(const GridVec2 & point, const GridDimen & size)const;
	//��ȡpoint������СΪsize��Ϊ�յ������rect
	friend void setCollisionPos(TMXTiledMap* map,GridMap * gmap);
	friend Vec2 tileCoordFromPosition(Vec2 pos, TMXTiledMap* map);
	bool checkBuildingRectPosition(const GridRect & rect)const;

	//get destination beside occupied destination
	GridVec2 getNewDestination(const GridVec2 & position,const map<GridVec2,int> & destination) const;
};
	

#endif
