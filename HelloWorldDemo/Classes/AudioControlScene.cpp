#include "AudioControlScene.h"
#include "SimpleAudioEngine.h"
#include "GameMenuScene.h"
#include "ui/CocosGUI.h"
using namespace CocosDenshion;
USING_NS_CC;
using namespace ui;

Scene* AudioControl::createScene()
{
	auto scene = Scene::create();
	auto layer = AudioControl::create();
	scene->addChild(layer);
	return scene;
}

bool AudioControl::init() {
	if (!Layer::init()) 
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto Bg = Sprite::create("bg.png");
	Bg->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.5));
	this->addChild(Bg);

	auto Music_Text = Text::create("BGMusic","Times New Roman",36);
	Music_Text->setPosition(Vec2(visibleSize.width*0.4, visibleSize.height*0.7));
	this->addChild(Music_Text);
	
	auto Sound_Text = Text::create("Sound", "Times New Roman", 36);
	Sound_Text->setPosition(Vec2(visibleSize.width*0.4, visibleSize.height*0.4));
	this->addChild(Sound_Text);

	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(36);
	MenuItemFont* ItemBack = MenuItemFont::create("BACK",
		CC_CALLBACK_1(AudioControl::menuItemBackCallback, this));
	Menu* menu = Menu::create(ItemBack, NULL);
	menu->setPosition(Vec2(visibleSize.width*0.9, 20));
	this->addChild(menu, 1);

	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(36);
	auto MusicToggleItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(AudioControl::menuItem1Callback, this),
		MenuItemFont::create("ON"),
		MenuItemFont::create("OFF"),
		NULL
	);
	Menu* menu1 = Menu::create(MusicToggleItem, NULL);
	menu1->setPosition(Vec2(visibleSize.width*0.6, visibleSize.height*0.7));
	this->addChild(menu1);

	auto SoundToggleItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(AudioControl::menuItem2Callback, this),
		MenuItemFont::create("ON"),
		MenuItemFont::create("OFF"),
		NULL
	);
	Menu* menu2 = Menu::create(SoundToggleItem, NULL);
	menu2->setPosition(Vec2(visibleSize.width*0.6, visibleSize.height*0.4));
	this->addChild(menu2);

	return true;
}

void AudioControl::menuItemBackCallback(cocos2d::Ref*pSender)
{
	auto GM = GameMenu::createScene();
	auto transition = TransitionTurnOffTiles::create(1.0, GM);
	Director::getInstance()->pushScene(transition);
}

void AudioControl::menuItem1Callback(cocos2d::Ref*pSender)
{
	MenuItem* Item = (MenuItem*)pSender;
	log("Touch Menu Item %p", Item);
}

void AudioControl::menuItem2Callback(cocos2d::Ref*pSender)
{
	MenuItem* Item = (MenuItem*)pSender;
	log("Touch Menu Item %p", Item);
}