#include"ManufactureMenu.h"

bool ManufactureMenu::init()
{
	if (!Menu::init()) 
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	_buildingButton = MenuItemImage::create("ui/menu/NormalBuildingButton.png", "ui/menu/SelectedBuildingButton.png");
	_buildingButton->setAnchorPoint(Vec2(1, 0.5));
	_buildingButton->setScale(0.6);
	_buildingButton->setPosition(visibleSize.width, visibleSize.height * 0.4);

	_armyButton = MenuItemImage::create("ui/menu/NormalArmyButton.png", "ui/menu/SelectedArmyButton.png");
	_armyButton->setAnchorPoint(Vec2(1, 0.5));
	_armyButton->setScale(0.6);
	_armyButton->setPosition(visibleSize.width, visibleSize.height * 0.6);

	addChild(_buildingButton);
	addChild(_armyButton);

	alignItemsVerticallyWithPadding(visibleSize.height * 0.2);

	return true;
}

void ManufactureMenu::setBuildingCallBack(std::function<void(Ref*)> callBack)
{
	_buildingButton->setCallback(callBack);
}

void ManufactureMenu::setArmyCallBack(std::function<void(Ref*)> callBack)
{
	_armyButton->setCallback(callBack);
}