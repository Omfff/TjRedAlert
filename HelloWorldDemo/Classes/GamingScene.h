#ifndef __GAMING_SCENE_H__
#define __GAMING_SCENE_H__

#include"cocos2d.h"
#include"SystemHeader.h"
#include"GridMap.h"
#include"MouseRect.h"
#include"Money.h"
#include"Electricity.h"
#include"UnitManager.h"
#include"ManufactureMenu.h"
#include"Building.h"
#include"Fighter.h"
#include<map>


class GamingScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void update(float f)override;

	//based on mouse move, scroll the map
	void mapScroll();

	//press key to change map position
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)override;
	//release key to change _keyStatus
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)override;

	void keyPressedToMove(EventKeyboard::KeyCode keyCode);

	//void focusOnBase();

	//get touch point
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)override;
	//while moving the mouse, change the mouse renctangle
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)override;
	//get end point, and call select function
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)override;

	//check if we can manufacture some buildings to set opacity
	void updateBuildingMenu(float f);

	//check if we can manufacture some fight units to set opacity
	void updateArmyMenu(float f);

	CREATE_FUNC(GamingScene);

private:


	cocos2d::TMXTiledMap* _tiledMap;
	


	//int _playerID;
	Unit * unit=nullptr;
	UnitManager * _unitManager= nullptr;
	GridMap * _gridMap= nullptr;
	GameMessageGroup * _msgs= nullptr;

	//mouse rectangle to choose soldiers or buildings
	MouseRect* _mouseRect = nullptr;

	//record cursor positon for map scroll
	cocos2d::Point _mousePosition{ 0, 0 };

	//display and change money label
	Money* _money = nullptr;

	//display and change electricity label
	Electricity* _electricity = nullptr;

	//menu to manufacture buildings and soldiers
	ManufactureMenu* _manufactureMenu = nullptr;

	Layer* _menuSpriteLayer = nullptr;

	//record key status
	map<EventKeyboard::KeyCode, bool> _keyStatus;

	bool _keyUpdate = true;

};

#endif
