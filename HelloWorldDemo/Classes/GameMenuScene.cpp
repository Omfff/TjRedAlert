#include "GameMenuScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "AudioControlScene.h"
#include "HelpScene.h"
#include "HelloWorldScene.h"
#include "PreloadScene.h"
USING_NS_CC;
using namespace ui;
//using namespace CocosDenshion;

Scene* GameMenu::createScene()
{
	auto Scene = Scene::create();
	auto Layer = GameMenu::create();
	Scene->addChild(Layer);
	return Scene;
}

bool GameMenu::init() {
	if (!Layer::init()) {
		return false;
	}
	UserDefault::getInstance()->setBoolForKey(MUSIC_KEY, true);
	UserDefault::getInstance()->setBoolForKey(SOUND_KEY, true);
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto Title = LabelTTF::create("Red Alert", "Times New Roman", 48);
	Title->setPosition(Vec2(visibleSize.width / 2,
		visibleSize.height-Title->getContentSize ().height));
	this->addChild(Title,1);

	auto Background = Sprite::create("red alert.png");
	Background->setAnchorPoint(Vec2::ZERO);
	this->addChild(Background);

	auto Start_Button = Button::create("start.png");
	Start_Button->setPosition(Vec2(visibleSize.width - Start_Button->getContentSize().width,
		visibleSize.height*0.8));
	Start_Button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto Transition = TransitionProgressVertical::create(1.0, HelloWorld::createScene());
			Director::getInstance()->replaceScene(Transition);

			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
			SimpleAudioEngine::getInstance()->playEffect("Music/Click.wav");
		}
	});
	this->addChild(Start_Button);

	auto Set_Button = Button::create("set.png");
	Set_Button->setPosition(Vec2(visibleSize.width - Set_Button->getContentSize().width,
		visibleSize.height*0.6));
	Set_Button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto Transition = TransitionCrossFade::create(2.0, AudioControl::createScene());
			Director::getInstance()->pushScene(Transition);
			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
			SimpleAudioEngine::getInstance()->playEffect("Music/Click.wav");
		}
	});
	this->addChild(Set_Button);

	auto Service_button = Button::create("service.png");
	Service_button->setPosition(Vec2(visibleSize.width - Service_button->getContentSize().width,
		visibleSize.height*0.4));
	Service_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto Transition = TransitionFadeDown::create(1.0, HelpScene::createScene());
			Director::getInstance()->pushScene(Transition);
			if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
			SimpleAudioEngine::getInstance()->playEffect("Music/Click.wav");
		}
	});
	this->addChild(Service_button);

	auto Close_Button = Button::create("leave.png");
	Close_Button->setPosition(Vec2(visibleSize.width - Close_Button->getContentSize().width,
		visibleSize.height*0.2));
	Close_Button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
#if(CC_TARGET_PLATFORM==CC_PLAFORM_WP8)||(CC_TARGET_PLATFORM==CC_PLATFORM_WINRT)
			MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
			return;
#endif
			Director::getInstance()->end();
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
			exit(0);
#endif
		}
	});
	this->addChild(Close_Button);
	return true;
}

	/*MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(48);
	MenuItemFont* ItemStart = MenuItemFont::create("Start",
		CC_CALLBACK_1(GameMenu::menuItemStartCallback, this));
	Menu* menu = Menu::create(ItemStart,NULL);
	menu->setPosition(Vec2(visibleSize.width*0.25,visibleSize.height*0.65));
	this->addChild(menu,1);

	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(48);
	MenuItemFont* ItemSet = MenuItemFont::create("Setting",
		CC_CALLBACK_1(GameMenu::menuItemSetCallback, this));
	Menu* menu1 = Menu::create(ItemSet, NULL);
	menu1->setPosition(Vec2(visibleSize.width*0.75, visibleSize.height*0.65));
	this->addChild(menu1, 1);

	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(48);
	MenuItemFont* ItemCredit = MenuItemFont::create("Help",
		CC_CALLBACK_1(GameMenu::menuItemHelpCallback, this));
	Menu* menu2 = Menu::create(ItemCredit, NULL);
	menu2->setPosition(Vec2(visibleSize.width*0.25, visibleSize.height*0.35));
	this->addChild(menu2, 1);

	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(48);
	MenuItemFont* ItemQuit = MenuItemFont::create("Quit",
		CC_CALLBACK_1(GameMenu::menuItemQuitCallback, this));
	Menu* menu3 = Menu::create(ItemQuit, NULL);
	menu3->setPosition(Vec2(visibleSize.width*0.75, visibleSize.height*0.35));
	this->addChild(menu3, 1);*/
	
/*void GameMenu::menuItemStartCallback(Ref*pSender)
{
	auto PreLoadScene = HelloWorld::createScene();
	auto transition = TransitionFadeTR::create(1.0, PreLoadScene);
	Director::getInstance()->pushScene(transition);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/background music.wav", true);
}

void GameMenu::menuItemSetCallback(Ref*pSender)
{
	auto ACS = AudioControl::createScene();
	auto transition = TransitionTurnOffTiles::create(1.0, ACS);
	Director::getInstance()->pushScene(transition);
}

void GameMenu::menuItemQuitCallback(cocos2d::Ref*pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif 
}

void GameMenu::menuItemHelpCallback(cocos2d::Ref*pSender)
{
	auto HS = HelpScene::createScene();
	auto transition = TransitionTurnOffTiles::create(1.0, HS);
	Director::getInstance()->pushScene(transition);
}*/

