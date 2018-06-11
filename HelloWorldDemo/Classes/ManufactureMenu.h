#ifndef __MANUFACTURE_MENU_H__
#define __MANUFACTURE_MENU_H__

#include"cocos2d.h"
#include"SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

class ManufactureMenu :public cocos2d::Menu
{
public:

	CREATE_FUNC(ManufactureMenu);

	bool init()override;

	void setGICallBack(std::function<void(Ref*)>);
	void setAttackDogCallBack(std::function<void(Ref*)>);

	void setTankCallBack(std::function<void(Ref*)>);

	void setBaseCallBack(std::function<void(Ref*)>);
	void setPowerPlantCallBack(std::function<void(Ref*)>);
	void setBarrackCallBack(std::function<void(Ref*)>);
	void setWarFactoryCallBack(std::function<void(Ref*)>);
	void setOreRefineryCallBack(std::function<void(Ref*)>);

	void setBuildingCallBack(std::function<void(Ref*)>);
	void setArmyCallBack(std::function<void(Ref*)>);

	cocos2d::MenuItemImage* _buildingButton;
	cocos2d::MenuItemImage* _armyButton;


	cocos2d::MenuItemImage* _GIButton;
	cocos2d::MenuItemImage* _attackDogButton;

	cocos2d::MenuItemImage* _tankButton;

	cocos2d::MenuItemImage* _baseButton;
	cocos2d::MenuItemImage* _powerPlantButton;
	cocos2d::MenuItemImage* _barrackButton;
	cocos2d::MenuItemImage* _warFactoryButton;
	cocos2d::MenuItemImage* _oreRefineryButton;

};


#endif // !__MANUFACTURE_MENU_H__

