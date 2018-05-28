#ifndef  _GameMenu_Scene_H_
#define  _GameMenu_Scene_H_

#include "cocos2d.h"

class GameMenu :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuItemStartCallback(cocos2d::Ref*pSender);
	void menuItemSetCallback(cocos2d::Ref*pSender);
	void menuItemQuitCallback(cocos2d::Ref*pSender);
	void menuItemHelpCallback(cocos2d::Ref*pSender);
	CREATE_FUNC(GameMenu);
};
#endif