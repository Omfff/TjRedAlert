#ifndef  _Help_Scene_H_
#define  _Help_Scene_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SystemHeader.h"
using namespace CocosDenshion;
class HelpScene :public cocos2d::Scene
{
public:
	
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuItemBackCallback(cocos2d::Ref*pSender);
	CREATE_FUNC(HelpScene);
};

#endif