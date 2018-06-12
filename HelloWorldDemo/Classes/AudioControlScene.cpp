#include "AudioControlScene.h"
#include "SimpleAudioEngine.h"
#include "GameMenuScene.h"
#include "ui/CocosGUI.h"
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
	if (!Layer::init()) {
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto Bg = Sprite::create("bg.png");
	Bg->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.5));
	this->addChild(Bg);

	auto Music_Text = Text::create("Music","Times New Roman",36);
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
	menu->setPosition(Vec2(visibleSize.width*0.94, 20));
	this->addChild(menu, 1);

	//ÒôÐ§²¿·Ö
	
	auto MusicOn = MenuItemImage::create(
		"on.png", "on.png"
	);
	auto MusicOff = MenuItemImage::create(
		"off.png", "off.png"
	);
	auto MusicToggleItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(AudioControl::menuItemMusicCallback, this),
		MusicOff,
		MusicOn,
		NULL
	);
	MusicToggleItem->setPosition(Vec2(visibleSize.width*0.6, visibleSize.height*0.7));

    auto SoundOn = MenuItemImage::create(
		"on.png", "off.png"
	);
	auto SoundOff = MenuItemImage::create(
		"off.png", "off.png"
	);
	auto SoundToggleItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(AudioControl::menuItemEffectCallback, this),
		SoundOff,
		SoundOn,
		NULL
	);
	SoundToggleItem->setPosition(Vec2(visibleSize.width*0.6, visibleSize.height*0.4));

	Menu* option = Menu::create(SoundToggleItem, MusicToggleItem, NULL);
	option->setPosition(Vec2::ZERO);
	this->addChild(option);

	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY)) {
		MusicToggleItem->setSelectedIndex(0);
	}
	else {
		MusicToggleItem->setSelectedIndex(1);
	}
	if (defaults->getBoolForKey(SOUND_KEY)) {
		SoundToggleItem->setSelectedIndex(0);
	}
	else {
		SoundToggleItem->setSelectedIndex(1);
	}


	return true;
}
void AudioControl::menuItemBackCallback(cocos2d::Ref*pSender)
{
	//auto GM = GameMenu::createScene();
	//auto transition = TransitionPageTurn::create(1.0, GM,false);
	Director::getInstance()->pushScene(GameMenu::createScene());
	if(UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	SimpleAudioEngine::getInstance()->playEffect("Music/Click.wav");
}
void AudioControl::menuItemMusicCallback(cocos2d::Ref*pSender)
{
	auto MusicItem = (MenuItemToggle*)pSender;
	log("MusicItem %d", MusicItem->getSelectedIndex());

	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY)) {
		UserDefault::getInstance()->setBoolForKey(MUSIC_KEY, false);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}else {
		UserDefault::getInstance()->setBoolForKey(MUSIC_KEY, true);
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/Background music.wav");
	}
}
void AudioControl::menuItemEffectCallback(cocos2d::Ref*pSender)
{
	auto EffectItem = (MenuItemToggle*)pSender;
	log("EffectItem %d", EffectItem->getSelectedIndex());

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		UserDefault::getInstance()->setBoolForKey(SOUND_KEY, false);
	}else {
		UserDefault::getInstance()->setBoolForKey(SOUND_KEY, true);
		SimpleAudioEngine::getInstance()->playEffect("Music/Click.wav");
	}
}
void AudioControl::onEnter() {
	Layer::onEnter();
	log("AudioControl onEnter");
}
void AudioControl::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
	log("AudioControl onEnterTransitionDidFinish");
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/BGM.wav", true);
}
void AudioControl::onExit() {
	Layer::onExit();
	log("AudioControl onExit");
}
void AudioControl::onExitTransitionDidStart() {
	Layer::onExitTransitionDidStart();
	log("AudioControl onExitTransitionDidStart");
}
void AudioControl::cleanup() {
	Layer::cleanup();
	log("AudioControl cleanup");
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("Music/Background music.wav");
}


/*MenuItemFont::setFontName("Times New Roman");
MenuItemFont::setFontSize(36);
auto MusicToggleItem = MenuItemToggle::createWithCallback(
CC_CALLBACK_1(AudioControl::menuItemMusicCallback, this),
MenuItemFont::create("ON"),
MenuItemFont::create("OFF"),
NULL
);
Menu* menu1 = Menu::create(MusicToggleItem, NULL);
menu1->setPosition(Vec2(visibleSize.width*0.6, visibleSize.height*0.7));
this->addChild(menu1);

auto SoundToggleItem = MenuItemToggle::createWithCallback(
CC_CALLBACK_1(AudioControl::menuItemEffectCallback, this),
MenuItemFont::create("ON"),
MenuItemFont::create("OFF"),
NULL
);
Menu* menu2 = Menu::create(SoundToggleItem, NULL);
menu2->setPosition(Vec2(visibleSize.width*0.6, visibleSize.height*0.4));
this->addChild(menu2);*/