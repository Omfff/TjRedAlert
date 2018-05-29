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
	if (!Layer::init()) {
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
	
	/*auto Music_Slider = Slider::create();
	Music_Slider->loadBarTexture("sliderTrack.png");
	Music_Slider->setScale(0.5);
	Music_Slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	Music_Slider->setScale(0.4);
	Music_Slider->loadProgressBarTexture("sliderProgress.png");
	Music_Slider->setScale(0.5);
	float MusicPercent = UserDefault::getInstance()->getFloatForKey("musicPercent");
	if (MusicPercent == 0.0f) {
		MusicPercent = 100.0f;
	}
	Music_Slider->setPercent(MusicPercent);
	Music_Slider->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.7));
	Music_Slider->addEventListener([=](Ref* pSender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			int percent = Music_Slider->getPercent();
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(percent) / 100);
			UserDefault::getInstance()->setFloatForKey("(MusicPercent", percent);
		}
	});
	this->addChild(Music_Slider);

	auto Effects_Slider = Slider::create();
	Effects_Slider->loadBarTexture("sliderTrack.png");
	Effects_Slider->setScale(0.5);
	Effects_Slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	Effects_Slider->setScale(0.5);
	Effects_Slider->loadProgressBarTexture("sliderProgress.png");
	Effects_Slider->setScale(0.5);
	float EffectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
	if (EffectPercent == 0.0f) {
		EffectPercent = 100.0f;
	}
	Effects_Slider->setPercent(EffectPercent);
	Effects_Slider->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.4));
	Effects_Slider->addEventListener([=](Ref* pSender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			int Percent = Effects_Slider->getPercent();
			SimpleAudioEngine::getInstance()->setEffectsVolume(float(Percent) / 100);
			UserDefault::getInstance()->setFloatForKey("effectPercent", Percent);
		}
	});
	this->addChild(Effects_Slider);*/

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