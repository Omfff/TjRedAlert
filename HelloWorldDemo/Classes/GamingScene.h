#ifndef __GAMING_SCENE_H__
#define __GAMING_SCENE_H__

#include"cocos2d.h"
#include"MouseRect.h"
#include"Money.h"
#include"Electricity.h"

class GamingScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void update(float f)override;

	//based on mouse move, scroll the map
	void mapScroll();

	//get touch point
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)override;
	//while moving the mouse, change the mouse renctangle
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)override;
	//get end point, and call select function
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)override;


	CREATE_FUNC(GamingScene);

private:

	cocos2d::TMXTiledMap* _tiledMap;
	//cocos2d::TMXLayer* _collidable;

	//int _playerID;

	MouseRect* _mouseRect = nullptr;

	cocos2d::Point _mousePosition{ 0, 0 };

	Money* _money = nullptr;

	Electricity* _electricity = nullptr;

};


#endif
