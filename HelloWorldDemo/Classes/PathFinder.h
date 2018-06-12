
#ifndef __PATH_FINDER_H__
#define __PATH_FINDER_H__

#include<vector>
#include"SystemHeader.h"
#include"GridMap.h"


class Grid
{
public:
	int getStatus();
	void setStatus(int status);

	int getXPosition();
	void setXPosition(int xPosition);

	int getYPosition();
	void setYPosition(int xPosition);

	int getGValue();
	void setGValue(int GValue);

	int getHValue();
	void setHValue(int HValue);

	int getFValue();
	void setFValue(int FValue);

	Grid* getParent();
	void setParent(Grid * parentGrid);

private:
	int _status = UNOCCUPIED;
	int _xPosition = 0;
	int _yPosition = 0;
	int _GValue = 0;
	int _HValue = 0;
	int _FValue = 0;
	Grid* _parentGrid = nullptr;
};

class PathFinder
{
public:
	//mark if grid is collidable
	//start point(x1, y1) 
	//ended point(x2, y2)
	PathFinder(std::vector<std::vector<int>> & barrierMap, int x1, int y1, int x2, int y2);

	//search if there exisits path
	void searchPath();

	//generate the path
	void generatePath();

	//return path of grid pinoit
	std::vector<GridVec2> getPath();

private:

	//select next grid with the smallest F Value
	Grid* selectNextGrid();

	//return if grid exists in open list
	bool isInOpenList(Grid & grid);

	//return if grid exists in close list
	bool isInCloseList(Grid & grid);

	//remove a grid from open list
	void removeFromOpenList(Grid * grid);

	//return Euclidean distance between grid1 and grid2
	int calculateEuclideanDistance(Grid & grid1, Grid & grid2);

	//return Manhattan distance between grid1 and grid2
	int calculateManhattanDistance(Grid & grid1, Grid & grid2);

	//return if the grid is available
	bool isAvailable(Grid & grid);

	//return if the grid is on the corner of something collidable
	bool checkCorner(Grid & grid1, Grid & grid2);

	//find the next step in surrounded grid
	void checkSurroundGrid(Grid & grid);

	int _mapWidth;
	int _mapHeight;
	std::vector<std::vector<Grid>> _mapGrid;
	Grid* _start;
	Grid* _end;
	std::vector<Grid *> _openList;
	std::vector<Grid *> _closeList;
	std::vector<GridVec2> _path;

};

#endif // !__PATH_FINDER_H__


