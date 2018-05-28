#ifndef  _AudioControl_Scene_H_
#define  _AudioControl_Scene_H_

#include "cocos2d.h"

class AudioControl :public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuItemBackCallback(cocos2d::Ref*pSender);
	void menuItem1Callback(cocos2d::Ref*pSender);
	void menuItem2Callback(cocos2d::Ref*pSender);
	CREATE_FUNC(AudioControl);
};
#endif