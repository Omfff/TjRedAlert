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
#include"SimpleAudioEngine.h"
#include"Fighter.h"
#include"SocketServer.h"
#include"SocketClient.h"
#include<map>

USING_NS_CC;
using namespace CocosDenshion;

class GamingScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene * createScene(socket_server * socketServer, boost::shared_ptr<talk_to_server> socketClient);

	static GamingScene * create(socket_server * socketServer, boost::shared_ptr<talk_to_server> socketClient);

	virtual bool init(socket_server * socketServer, boost::shared_ptr<talk_to_server> socketClient);

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

	void updateFightUnitWaitingNum(float dt);

private:
	socket_server * _socketServer;
	boost::shared_ptr<talk_to_server> _socketClient;

	cocos2d::TMXTiledMap* _tiledMap;
	
	//Unit * unit=nullptr;
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
	
	LabelBMFont *  _soliderWaitingNum;
	LabelBMFont *  _tankWaitingNum;
	LabelBMFont *  _attackdogWaitingNum;


};

#endif
