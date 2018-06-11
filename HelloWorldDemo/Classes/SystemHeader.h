
#ifndef __SYSTEM_HEADER_H__
#define __SYSTEM_HEADER_H__

//Money.h
const int INITIAL_MONEY = 10000;
const int MONEY_IN_PERIOD = 0;
const int PERIOD = 20;


//GamingScene.cpp --void scroll()
const int BOX_EDGE_WIDTH_SMALL = 20;
const int BOX_EDGE_WIDTH = 40;
const int BOX_EDGE_HEIGHT_SMALL = 20;
const int BOX_EDGE_HEIGHT = 40;

const int SCROLL_DISTANCE = 4;

//GamingScene.cpp--bool init()--manufacturemenu
enum {
	BASE_TAG = 101,
	POWER_PLANT_TAG,
	BARRACK_TAG,
	WAR_FACTORY_TAG,
	ORE_REFINERY_TAG,
	GI_TAG,
	ATTACK_DOG_TAG,
	TANK_TAG
};

typedef enum {
	BASE = 0,
	POWERPLANT,
	BARRACKS,
	WARFACTORY,
	OREREFINERY,
	GI,
	ATTACKDOG,
	TANK
}UnitTypes;
typedef enum {
	NULLCAMP = 0,
	RED,
	BLUE,
	GREEN,
	YELLOW
}CampTypes;

const int TILED_PIXEL = 32;

//PathFinder
enum { UNOCCUPIED, OCCUPIED, START, END, INOPENLIST, INCLOSELIST };
const int DIRECTION[8][2] = { { 1, 0 },{ 1, -1 },{ 0, -1 },{ -1, -1 },{ -1, 0 },{ -1, 1 },{ 0, 1 },{ 1, 1 } };
const int STRAIGHT_PATH = 10;
const int OBLIQUE_PATH = 14;

#endif // !__SYSTEM_HEADER_H__

