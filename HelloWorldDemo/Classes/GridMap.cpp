#include"GridMap.h"
vector<GridVec2> orientations = { GridVec2(1, 1), GridVec2(1, -1), GridVec2(-1, -1), GridVec2(-1, 1) };
GridVec2::GridVec2(const Vec2 & point)
{
	_x = point.x;
	_y = point.y;
}
GridVec2 operator +(const GridVec2 & point1, const GridVec2 &point2)
{
	 GridVec2 resultPoint(point1._x + point2._x, point1._y + point2._y);
	 return resultPoint;
}
GridVec2 operator -(const GridVec2 & point1, const GridVec2 &point2)
{
	GridVec2 resultPoint(point1._x - point2._x, point1._y - point2._y);
	return resultPoint;
}
bool operator==(const GridVec2 & point1, const GridVec2 & point2)
{
	if (point1._x == point2._x && point1._y == point2._y)
		return true;
	else
		return false;
}
bool operator ==(const GridDimen & size1, const GridDimen & size2)
{
	if (size1._height == size2._height&&size1._width == size2._width)
		return true;
	else
		return false;
}
bool operator ==(const GridRect & rect1,const GridRect & rect2)
{
	if (rect1._oriPoint == rect2._oriPoint&&rect1._dimen==rect2._dimen)
		return true;
	else
		return false;
}
bool GridRect::containsPoint(const GridVec2 & point)const
{
	if (point._x >= _oriPoint._x&&point._y >= _oriPoint._y&&
		point._x < _oriPoint._x + _dimen._width&&point._y < _oriPoint._y + _dimen._height)
		return true;
	else
		return false;
}
bool GridRect::intersectsRect(const GridRect & rect)const
{
	if (rect.containsPoint(_oriPoint) && containsPoint(rect._oriPoint))
		return true;
	else
		return false;
}
bool GridRect::insideRect(const GridRect & rect)const
{
	if (this->containsPoint(rect._oriPoint)
		&& this->containsPoint(GridVec2(rect._oriPoint._x + rect._dimen._width - 1,
			rect._oriPoint._y + rect._dimen._height - 1)))
		return true;
	else
		return false;
}
GridMap::GridMap():_worldRect(0, 0, _MAP_WIDTH, _MAP_HEIGHT)
{
	_barrierMap.resize(_MAP_WIDTH);
	for (auto & e : _barrierMap)
		e.resize(_MAP_HEIGHT);
	for (auto & e : _barrierMap)
		for (auto & ie : e)
			ie = 0;
	_ladderRect1 = GridRect(13,5,4,5);
	_ladderRect2 = GridRect(107,5,4,5);
	_ladderRect3 = GridRect(107,51,4,5);
	_ladderRect4 = GridRect(13,51,4,5);
	for (int i = 0; i < 125; i++)
		for (int j = 0; j < 62; j++)
			_findPathMap[i][j] = 0;
}
bool GridMap::unitCoordStore(int unitId, const GridVec2 &position)
{
	if (!checkPointPosition(position))
	{
		_unitCoord[unitId] = position;
		unitPositionOccupy(unitId, position);
		return true;
	}
	else
		return false;
}
bool GridMap::unitCoordStore(int unitId, const GridRect & rect)
{
	if (!checkRectPosition(rect))
	{
		_unitCoord[unitId] = rect._oriPoint;
		unitPositionOccupy(unitId, rect);
		return true;
	}
	else
		return false;
}
void GridMap::unitPositionOccupy(int unitId, const GridVec2& position)
{
	_barrierMap[position._x][position._y] = unitId;
	_findPathMap[position._x][position._y] = 1;
}
void GridMap::unitPositionOccupy(int unitId, const GridRect & rect)
{
	for (int x = rect._oriPoint._x; x < rect._oriPoint._x + rect._dimen._width; x++)
		for (int y = rect._oriPoint._y; y < rect._oriPoint._y + rect._dimen._height; y++)
		{
			_barrierMap[x][y] = unitId;
			_findPathMap[x][y] = 1;
		}
}
void GridMap::unitLeavePosition(int unitId, const GridVec2& position)
{
	_barrierMap[position._x][position._y] = 0;
	_findPathMap[position._x][position._y] = 0;

}
void GridMap::unitLeavePosition(const GridRect & rect)
{
	for (int x = rect._oriPoint._x; x < rect._oriPoint._x + rect._dimen._width; x++)
		for (int y = rect._oriPoint._y; y < rect._oriPoint._y + rect._dimen._height; y++)
		{
			_barrierMap[x][y] = 0;
			_findPathMap[x][y] = 0;
		}
}
void GridMap::unitCoordRemove(int unitId,GridRect unitRect )
{
	//unitLeavePosition?????????????????????????????????????????????????????????????????????
	unitLeavePosition(unitRect);
	map<int, GridVec2>::iterator iter = _unitCoord.find(unitId);
	_unitCoord.erase(iter);
}

bool GridMap::checkPointPosition(const GridVec2 & point)const
{
	if (_worldRect.containsPoint(point))
		if (_barrierMap[point._x][point._y] != 0)
			return true;
		else
			return false;
	else
		return true;
}
bool GridMap::checkBuildingRectPosition(const GridRect & rect)const
{
	if (_worldRect.insideRect(rect))
	{
		if (rect.intersectsRect(_ladderRect1) || rect.intersectsRect(_ladderRect2)
			|| rect.intersectsRect(_ladderRect3) || rect.intersectsRect(_ladderRect4))
			return true;

		int xEnd = rect._oriPoint._x + rect._dimen._width;
		int yEnd = rect._oriPoint._y + rect._dimen._height;
		for (int xBegin = rect._oriPoint._x; xBegin < xEnd; xBegin++)
			for (int yBegin = rect._oriPoint._y; yBegin <yEnd; yBegin++)
			{
				if (_barrierMap[xBegin][yBegin] != 0)
					return true;
			}
		return false;
	}
	else
		return true;
}
bool GridMap::checkRectPosition(const GridRect & rect)const
{
	if (_worldRect.insideRect(rect))
	{
		int xEnd = rect._oriPoint._x + rect._dimen._width;
		int yEnd = rect._oriPoint._y + rect._dimen._height;
		for (int xBegin = rect._oriPoint._x; xBegin < xEnd; xBegin++)
			for (int yBegin = rect._oriPoint._y; yBegin <yEnd ; yBegin++)
			{
				if (_barrierMap[xBegin][yBegin] != 0)
					return true;
			}
		return false;
	}
	else
		return true;
}
int GridMap::getUnitIdAt(const GridVec2& position)const
{
	return _barrierMap[position._x][position._y];
}
set<int> GridMap::getUnitIdAt(const GridRect & range)const
{
	set<int> idGroup;
	int xEnd = (range._oriPoint._x + range._dimen._width) > _MAP_WIDTH ? _MAP_WIDTH : (range._oriPoint._x + range._dimen._width);
	int yEnd = (range._oriPoint._y + range._dimen._height) > _MAP_HEIGHT ? _MAP_HEIGHT : (range._oriPoint._y + range._dimen._height);
	for (int xBegin = range._oriPoint._x; xBegin <=xEnd; xBegin++)
		for (int yBegin = range._oriPoint._y; yBegin <=yEnd; yBegin++)
		{
			if (_barrierMap[xBegin][yBegin] != 0&& _barrierMap[xBegin][yBegin] != _NO_PASS)
			{
				idGroup.insert(_barrierMap[xBegin][yBegin]);
			}
			else
				continue;	
		}
	return idGroup;
}
GridVec2 GridMap::getEmptyPointNearby(const GridVec2 & position)const
{
	if (!checkPointPosition(position))
		return position;
	else
	{
		for (int i = 1; i <= _MAP_HEIGHT + _MAP_HEIGHT; i++)
		{
			GridVec2 tempPoint = position + GridVec2(-i, 0);
			for(int oritat=0;oritat<4;oritat++)
				for (int k = 1; k <= i; k++)
				{
					tempPoint = tempPoint + orientations[oritat];
					if (!checkPointPosition(tempPoint))
						return tempPoint;
				}
		}
	}
	return GridVec2(-100, -100);
}
GridRect GridMap::getEmptyRectNearby(const GridVec2 & point, const GridDimen & size)const
{
	if (!checkPointPosition(point))
	{
		if (!checkRectPosition(GridRect(point, size)))
			return GridRect(point, size);
	}
	else
	{
		for (int i = 1; i <= _MAP_HEIGHT + _MAP_HEIGHT; i++)
		{
			GridVec2 tempPoint = point + GridVec2(-i, 0);
			for (int oritat = 0; oritat<4; oritat++)
				for (int k = 1; k <= i; k++)
				{
					tempPoint = tempPoint + orientations[oritat];
					if ((!checkPointPosition(tempPoint)) &&
						(!checkRectPosition(GridRect(tempPoint, size))))
						return GridRect(tempPoint, size);
				}
		}
	}
	return GridRect(-100, -100, 0, 0);
}
void setCollisionPos(TMXTiledMap* map,GridMap * gmap)
{
	auto collidable = map->getLayer("collidable");
	Value prop;
	ValueMap propValueMap;
	string collision;
	auto barrier = Sprite::create("barrier.png");
	barrier->setAnchorPoint(Point(0, 0));
	barrier->setPosition(Vec2(0,0));
	gmap->addChild(barrier);
	for(int i=0;i<map->getMapSize().width;i++)
		for (int j = 0; j< map->getMapSize().height; j++)
		{
			if (i == 0 || j == 0||i==124||j==61)
			{
				auto barrier = Sprite::create("barrier.png");
				barrier->setAnchorPoint(Point(0, 0));
				barrier->setPosition(Vec2(i * 32, (62 - j - 1) * 32));
				gmap->addChild(barrier);
			}
			Vec2 tileCoord = Vec2(i, j);// tileCoordFromPosition(Vec2(i, j), map);
			int tileGid = collidable->getTileGIDAt(tileCoord);
			
			if (tileGid > 0)
			{
				/*prop = map->getPropertiesForGID(tileGid);
				propValueMap = prop.asValueMap();

				collision = propValueMap["Collidable"].asString();*/
				auto barrier = Sprite::create("barrier.png");
				barrier->setAnchorPoint(Point(0,0));
				barrier->setPosition(Vec2(i * 32, (62-j-1) * 32));
				gmap->addChild(barrier);

				gmap->_barrierMap[tileCoord.x][62-tileCoord.y-1] = _NO_PASS;
				gmap->_findPathMap[(int)tileCoord.x][int(62 - tileCoord.y - 1)] = 1;
				
			}
		}
}
Vec2 tileCoordFromPosition(Vec2 pos, TMXTiledMap* map)
{
	int x = pos.x / map->getTileSize().width;
	int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y) / map->getTileSize().height;
	return Vec2(x, y);
}