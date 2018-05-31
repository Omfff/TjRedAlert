#ifndef __SYSTEM_HEADER_H__
#define __SYSTEM_HEADER_H__

//Money.h
#define INITIAL_MONEY 10000
#define MONEY_IN_PERIOD 100
#define PERIOD 20


//GamingScene.cpp --void scroll()
#define BOX_EDGE_WIDTH_SMALL 20
#define BOX_EDGE_WIDTH 40
#define BOX_EDGE_HEIGHT_SMALL 20
#define BOX_EDGE_HEIGHT 40

#define SCROLL_DISTANCE 10

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



#endif // !__SYSTEM_HEADER_H__

