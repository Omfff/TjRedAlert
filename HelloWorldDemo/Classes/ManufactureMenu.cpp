#include"ManufactureMenu.h"

bool ManufactureMenu::init()
{
	if (!Menu::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	_buildingButton = MenuItemFont::create("Building");
	_buildingButton->setAnchorPoint(Vec2(1, 1));
	_buildingButton->setScale(0.6);
	_buildingButton->setPosition(visibleSize.width, visibleSize.height - 2 * _buildingButton->getContentSize().height);
	//buildingButton->setPosition(getContentSize().width, getContentSize().height - _buildingButton->getContentSize().height);

	addChild(_buildingButton);

	alignItemsVertically();

	/*_GIButton = cocos2d::MenuItemImage::create();

	_attackDogButton = cocos2d::MenuItemImage::create();

	_tankButton = cocos2d::MenuItemImage::create();

	_baseButton = cocos2d::MenuItemImage::create("units/base_1.png",
												"units/base_1.png",
												"units/base_1.png");
	_baseButton->setScale(0.8);
	_baseButton->setAnchorPoint(Vec2(1,1));
	_baseButton->setPosition(getContentSize().width, getContentSize().height - _baseButton->getContentSize().height);


	_powerPlantButton = cocos2d::MenuItemImage::create();

	_barrackButton = cocos2d::MenuItemImage::create();

	_warFactoryButton = cocos2d::MenuItemImage::create();

	_oreRefineryButton = cocos2d::MenuItemImage::create();


	addChild(_GIButton);
	addChild(_attackDogButton);

	addChild(_tankButton);

	addChild(_baseButton);
	addChild(_powerPlantButton);
	addChild(_barrackButton);
	addChild(_warFactoryButton);
	addChild(_oreRefineryButton);

	alignItemsVertically();*/

	return true;
}

void ManufactureMenu::setBuildingCallBack(std::function<void(Ref*)> callBack)
{
	_buildingButton->setCallback(callBack);
}

void ManufactureMenu::setArmCallBack(std::function<void(Ref*)> callBack)
{
	_armButton->setCallback(callBack);
}

void ManufactureMenu::setGICallBack(std::function<void(Ref*)> callBack)
{
	_GIButton->setCallback(callBack);
}

void ManufactureMenu::setAttackDogCallBack(std::function<void(Ref*)> callBack)
{
	_attackDogButton->setCallback(callBack);
}

void ManufactureMenu::setTankCallBack(std::function<void(Ref*)> callBack)
{
	_tankButton->setCallback(callBack);
}

void ManufactureMenu::setBaseCallBack(std::function<void(Ref*)> callBack)
{
	_baseButton->setCallback(callBack);
}

void ManufactureMenu::setPowerPlantCallBack(std::function<void(Ref*)> callBack)
{
	_powerPlantButton->setCallback(callBack);
}

void ManufactureMenu::setBarrackCallBack(std::function<void(Ref*)> callBack)
{
	_barrackButton->setCallback(callBack);
}

void ManufactureMenu::setWarFactoryCallBack(std::function<void(Ref*)> callBack)
{
	_warFactoryButton->setCallback(callBack);
}

void ManufactureMenu::setOreRefineryCallBack(std::function<void(Ref*)> callBack)
{
	_oreRefineryButton->setCallback(callBack);
}
