#ifndef __MANUFACTURE_MENU_H__
#define __MANUFACTURE_MENU_H__

#include"cocos2d.h"
#include"SystemHeader.h"

USING_NS_CC;

class ManufactureMenu :public cocos2d::Menu
{
public:

	CREATE_FUNC(ManufactureMenu);

	bool init()override;

	void setBuildingCallBack(std::function<void(Ref*)>);
	void setArmyCallBack(std::function<void(Ref*)>);
	
	MenuItemImage* _buildingButton;
	MenuItemImage* _armyButton;

};


#endif // !__MANUFACTURE_MENU_H__