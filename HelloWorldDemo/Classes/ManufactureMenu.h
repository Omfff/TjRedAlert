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
	

	MenuItemImage* _buildingButton;
	MenuItemImage* _armyButton;


	MenuItemImage* _GIButton;
	MenuItemImage* _attackDogButton;

	MenuItemImage* _tankButton;

	MenuItemImage* _baseButton;
	MenuItemImage* _powerPlantButton;
	MenuItemImage* _barrackButton;
	MenuItemImage* _warFactoryButton;
	MenuItemImage* _oreRefineryButton;

};


#endif // !__MANUFACTURE_MENU_H__