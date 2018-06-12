#ifndef  _AudioControl_Scene_H_
#define  _AudioControl_Scene_H_
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SystemHeader.h"
using namespace CocosDenshion;
class AudioControl :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	virtual void cleanup();

	void menuItemBackCallback(cocos2d::Ref*pSender);
	void menuItemMusicCallback(cocos2d::Ref*pSender);
	void menuItemEffectCallback(cocos2d::Ref*pSender);
	CREATE_FUNC(AudioControl);
};
#endif