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

//将cocos自带的vec2（float）转为类型为int的坐标值
//并重载了加减乘等号运算符
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
//将cocos自带的size类型转化为int的dimen
//重载了==运算符
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
//将cocos自带的rect类型转为数据类型为int的
//有三种初始化的方式
class GridRect
{
public:
	GridVec2 _oriPoint;
	GridDimen _dimen;
	//三种初始化方式
	GridRect() {}
	GridRect(float x, float y, float width, float height) :_oriPoint(x, y), _dimen(width, height) {}
	GridRect(const GridVec2 & point, const GridDimen & size) :_oriPoint(point), _dimen(size) {}
	GridRect(const Rect & rect) :_oriPoint(rect.origin), _dimen(rect.size) {};
	//重载了==运算符
	friend bool operator ==(const GridRect & rect1,const GridRect &rect2);
	//point是否在该GridRect内，若在返回true，否则返回false
	bool containsPoint(const GridVec2 & point)const;
	//rect是否与该GridRect相交
	bool intersectsRect(const GridRect & rect)const;
	//rect是否在该GridRect内
	bool insideRect(const GridRect &rect)const;
};
//网格地图
class GridMap:public Layer
{
protected:
	//UnitManager * _unitManager;
	map<int, GridVec2> _unitCoord;
	//储存各单位的网格坐标
	vector <vector<int>> _barrierMap;
	//存储网格地图上每个格子的状态 0为空 其余的代表unit的id或者地图障碍物体
	
	GridRect _worldRect;
	//（0，0） width=125 height=62
	GridRect _ladderRect1;
	GridRect _ladderRect2;
	GridRect _ladderRect3;
	GridRect _ladderRect4;
public:

	vector <vector<int>> _findPathMap;
	CREATE_FUNC(GridMap);
	GridMap();
	//初始化
	bool unitCoordStore(int unitId, const GridVec2 &position);
	bool unitCoordStore(int unitId, const GridRect & rect);
	//存储单位的位置信息 返回true为成功
	void unitPositionOccupy(int unitId, const GridVec2& position);
	void unitPositionOccupy(int unitId, const GridRect & rect);
	//在存储单位的位置信息后会自动调用单位占用网格地图上的网格
	void unitLeavePosition(int unitId, const GridVec2& posioton);
	void unitLeavePosition(const GridRect & rect);
	//在移除单位的位置信息后会自动调用单位占用网格的清空
	//（自动调用还没写！！！！）
	void unitCoordRemove(int unitId,GridRect unitRect);
	//移除单位信息（一般是在单位灭亡时调用）
	int getUnitIdAt(const GridVec2& position)const;
	//获取position位置上的单位信息
	set <int> getUnitIdAt(const GridRect & range)const;
	//获取一定范围内的所有单位id
	bool checkPointPosition(const GridVec2 & point)const;
	//判断point在网格地图上是否为空 若不为空则返回true！！！！若为空则返回false 
	//（若point超出地图则也返回true！！！）
	bool checkRectPosition(const GridRect & rect)const;
	//判断rect在网格地图上是否为空 若不为空则返回true！！！（若rect不在地图中则也返回true）
	GridVec2 getEmptyPointNearby(const GridVec2 & position)const;
	//获取point附近空的网格的坐标
	GridRect getEmptyRectNearby(const GridVec2 & point, const GridDimen & size)const;
	//获取point附近大小为size且为空的区域的rect
	friend void setCollisionPos(TMXTiledMap* map,GridMap * gmap);
	friend Vec2 tileCoordFromPosition(Vec2 pos, TMXTiledMap* map);
	bool checkBuildingRectPosition(const GridRect & rect)const;

	//get destination beside occupied destination
	GridVec2 getNewDestination(const GridVec2 & position,const map<GridVec2,int> & destination) const;
};
	

#endif
