#include"PathFinder.h"

int Grid::getStatus()
{
	return _status;
}

void Grid::setStatus(int status)
{
	_status = status;
}

int Grid::getXPosition()
{
	return _xPosition;
}

void Grid::setXPosition(int xPosition)
{
	_xPosition = xPosition;
}

int Grid::getYPosition()
{
	return _yPosition;
}

void Grid::setYPosition(int yPosition)
{
	_yPosition = yPosition;
}

int Grid::getGValue()
{
	return _GValue;
}

void Grid::setGValue(int GValue)
{
	_GValue = GValue;
}

int Grid::getHValue()
{
	return _HValue;
}

void Grid::setHValue(int HValue)
{
	_HValue = HValue;
}

int Grid::getFValue()
{
	return _FValue;
}

void Grid::setFValue(int FValue)
{
	_FValue = FValue;
}

Grid* Grid::getParent()
{
	return _parentGrid;
}

void Grid::setParent(Grid * parentGrid)
{
	_parentGrid = parentGrid;
}

PathFinder::PathFinder(std::vector<std::vector<int>> & barrierMap, int x1, int y1, int x2, int y2)
{
	_mapWidth = barrierMap.size();
	_mapHeight = barrierMap[0].size();

	//initial the _mapGrid
	std::vector<Grid> column(_mapHeight, Grid());
	_mapGrid.assign(_mapWidth, column);

	for(int i = 0; i < _mapWidth; ++i)
		for (int j = 0; j < _mapHeight; ++j) {
			_mapGrid[i][j].setXPosition(i);
			_mapGrid[i][j].setYPosition(j);
			if (barrierMap[i][j] == UNOCCUPIED) {
				_mapGrid[i][j].setStatus(UNOCCUPIED);
			} else {
				_mapGrid[i][j].setStatus(OCCUPIED);
			}
		}

	_mapGrid[x1][y1].setStatus(START);
	_mapGrid[x2][y2].setStatus(END);
	_start = &_mapGrid[x1][y1];
	_end = &_mapGrid[x2][y2];
}

Grid* PathFinder::selectNextGrid()
{
	Grid * nextGrid = _openList[0];

	for (auto p : _openList) {
		if (p->getFValue() < nextGrid->getFValue()) {
			nextGrid = p;
		}
	}

	return nextGrid;
}

bool PathFinder::isInOpenList(Grid & grid)
{
	if (grid.getStatus() == INOPENLIST) {
		return true;
	}
	return false;
}

bool PathFinder::isInCloseList(Grid & grid)
{
	if (grid.getStatus() == INCLOSELIST || grid.getStatus() == START) {
		return true;
	}
	return false;
}

void PathFinder::removeFromOpenList(Grid * grid)
{
	std::vector<Grid *>::iterator iter = find(_openList.begin(), _openList.end(), grid);
	if (iter != _openList.end()){
		_openList.erase(iter);
	}
}

int PathFinder::calculateEuclideanDistance(Grid & grid1, Grid & grid2)
{
	if (grid1.getXPosition() == grid2.getXPosition() || grid1.getYPosition() == grid2.getYPosition()) {
		return grid2.getGValue() + STRAIGHT_PATH;
	}
	return grid2.getGValue() + OBLIQUE_PATH;
}

int PathFinder::calculateManhattanDistance(Grid & grid1, Grid & grid2)
{
	return (abs(grid1.getXPosition() - grid2.getXPosition()) + abs(grid1.getYPosition() - grid2.getYPosition())) * STRAIGHT_PATH;
}

bool PathFinder::isAvailable(Grid & grid)
{
	if (isInCloseList(grid)) {
		return false;
	}
	else if (grid.getStatus() == OCCUPIED) {
		return false;
	}
	return true;
}

bool PathFinder::checkCorner(Grid & grid1, Grid & grid2)
{
	if (grid1.getXPosition() == grid2.getXPosition() || grid1.getYPosition() == grid2.getYPosition()) {
		return false;
	}
	else if (_mapGrid[grid1.getXPosition()][grid2.getYPosition()].getStatus() == OCCUPIED
		|| _mapGrid[grid2.getXPosition()][grid1.getYPosition()].getStatus() == OCCUPIED) {
		return true;
	}
	return false;
}

void PathFinder::checkSurroundGrid(Grid & grid)
{
	for (int i = 0; i < 8; ++i) {
		auto x = grid.getXPosition() + DIRECTION[i][0];
		auto y = grid.getYPosition() + DIRECTION[i][1];
		if (x >= 0 && x < _mapWidth && y >= 0 && y <= _mapHeight && checkCorner(grid, _mapGrid[x][y])
			&& isAvailable(_mapGrid[x][y]) && _mapGrid[x][y].getStatus() == UNOCCUPIED) {
			if (!isInOpenList(_mapGrid[x][y]) && !isInCloseList(_mapGrid[x][y])) {
				_mapGrid[x][y].setGValue(calculateEuclideanDistance(_mapGrid[x][y], grid));
				_mapGrid[x][y].setHValue(calculateManhattanDistance(_mapGrid[x][y], *_end));
				_mapGrid[x][y].setFValue(_mapGrid[x][y].getGValue() + _mapGrid[x][y].getHValue());

				_mapGrid[x][y].setStatus(INOPENLIST);
				_mapGrid[x][y].setParent(&grid);
				_openList.push_back(&_mapGrid[x][y]);
			}
			else if (isInOpenList(_mapGrid[x][y])) {
				if (calculateEuclideanDistance(_mapGrid[x][y], grid) < _mapGrid[x][y].getGValue()) {
					_mapGrid[x][y].setGValue(calculateEuclideanDistance(_mapGrid[x][y], grid));
					_mapGrid[x][y].setFValue(_mapGrid[x][y].getGValue() + _mapGrid[x][y].getHValue());
					_mapGrid[x][y].setParent(&grid);
				}
			}
		}
	}
}

void PathFinder::searchPath()
{
	Grid * currentGrid = nullptr;
	_openList.push_back(_start);
	while (_end->getStatus() != INCLOSELIST && !_openList.empty()) {
		currentGrid = selectNextGrid();
		checkSurroundGrid(*currentGrid);
		currentGrid->setStatus(INCLOSELIST);
		_closeList.push_back(currentGrid);
		removeFromOpenList(currentGrid);
	}
}

void PathFinder::generatePath()
{
	Grid * grid = _end;
	while (grid->getParent() != nullptr) {
		_path.push_back(GridVec2(grid->getXPosition(), grid->getYPosition()));
		grid = grid->getParent();
	}
}

std::vector<GridVec2> PathFinder::getPath()
{
	return _path;
}
