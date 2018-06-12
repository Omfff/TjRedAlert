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
using namespace CocosDenshion;

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

	//void focusOnBase();

	//get touch point
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)override;
	//while moving the mouse, change the mouse renctangle
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)override;
	//get end point, and call select function
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)override;

	virtual void update(float dt);
	virtual void update1(float dt);
	virtual void update2(float dt);
	
	virtual void hideloadbar();

	virtual void LoadingBar();
	virtual void LoadingBar(int i);
	virtual void LoadingBar(float j);


	CREATE_FUNC(GamingScene);

private:


	cocos2d::TMXTiledMap* _tiledMap;
	//½ø¶ÈÌõ
	float cu;
	int count = 0;
	Sprite * progressbgSprite = nullptr;
	Sprite * progressSprite = nullptr;

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

};


#endif
