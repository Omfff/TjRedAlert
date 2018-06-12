#include "GameMenuScene.h"
#include "ui/CocosGUI.h"
#include "HelpScene.h"

USING_NS_CC;
//using namespace ui;

Scene* HelpScene::createScene()
{
	return HelpScene::create();
}

bool HelpScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();


	auto help = Sprite::create("help.png");
	help->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2 ));
	this->addChild(help);

	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(36);
	MenuItemFont* ItemBack = MenuItemFont::create("BACK",
		CC_CALLBACK_1(HelpScene::menuItemBackCallback, this));
	Menu* menu = Menu::create(ItemBack, NULL);
	menu->setPosition(Vec2(visibleSize.width*0.9, 20));
	this->addChild(menu, 1);
	return true;
}

void HelpScene::menuItemBackCallback(cocos2d::Ref*pSender)
{
	auto GM = GameMenu::createScene();
	auto transition = TransitionTurnOffTiles::create(1.0, GM);
	Director::getInstance()->pushScene(transition);
	SimpleAudioEngine::getInstance()->playEffect("Music/Click.wav");
}