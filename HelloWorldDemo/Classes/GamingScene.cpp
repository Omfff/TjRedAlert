#include"GamingScene.h"

USING_NS_CC;

Scene* GamingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GamingScene::create();
	scene->addChild(layer);
	return scene;
}

bool GamingScene::init()
{
	if (!Layer::create()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();



	_tiledMap = TMXTiledMap::create("maps/TjRedAlertMap(1).tmx");
	addChild(_tiledMap, 0);



	_manufactureMenu = ManufactureMenu::create();
	_manufactureMenu->setPosition(Vec2(origin.x + visibleSize.width - 20, origin.y + visibleSize.height / 2));

	_manufactureMenu->setGICallBack([&](Ref*)
	{
		if (_money->checkMoney(150)) {
			_money->costMoney(150);
		}
	});

	_manufactureMenu->setAttackDogCallBack([&](Ref*)
	{
		if (_money->checkMoney(150)) {
			_money->costMoney(150);
		}
	});

	_manufactureMenu->setTankCallBack([&](Ref*)
	{
		if (_money->checkMoney(150)) {
			_money->costMoney(150);
		}
	});

	_manufactureMenu->setBaseCallBack([&](Ref*)
	{
		if (_money->checkMoney(2500)) {
			_money->costMoney(2500);
			_electricity->addElectricity(50);
		}
	});

	_manufactureMenu->setPowerPlantCallBack([&](Ref*)
	{
		if (_money->checkMoney(800)) {
			_money->costMoney(800);
			_electricity->addElectricity(200);
		}
	});

	_manufactureMenu->setBarrackCallBack([&](Ref*)
	{
		if (_money->checkMoney(800) && _electricity->checkElectricity(10)) {
			_money->costMoney(800);
			_electricity->costElectricity(10);
		}
	});

	_manufactureMenu->setWarFactoryCallBack([&](Ref*)
	{
		if (_money->checkMoney(2000) && _electricity->checkElectricity(50)) {
			_money->costMoney(2000);
			_electricity->costElectricity(50);
		}
	});

	_manufactureMenu->setOreRefineryCallBack([&](Ref*)
	{
		if (_money->checkMoney(2000) && _electricity->checkElectricity(40)) {
			_money->costMoney(2000);
			_money->addMoneyInPeriod(50);
			_electricity->costElectricity(40);
		}
	});

	addChild(_manufactureMenu, 10);



	auto moneyIcon = Sprite::create("ui/money/gold.png");
	moneyIcon->setPosition(visibleSize.width - 120, 20);
	moneyIcon->setScale(0.04);
	addChild(moneyIcon, 1);

	_money = Money::create();
	_money->setPosition(visibleSize.width - 80, 20);
	_money->schedule(schedule_selector(Money::update));
	addChild(_money, 1);
	
	_electricity = Electricity::create();
	_electricity->setPosition(visibleSize.width - 80, 80);
	addChild(_electricity, 1);



	_mouseRect = MouseRect::create();
	_mouseRect->setVisible(false);
	_tiledMap->addChild(_mouseRect, 60);

	schedule(schedule_selector(GamingScene::update));

	auto mouseListener = EventListenerTouchOneByOne::create();
	mouseListener->onTouchBegan = CC_CALLBACK_2(GamingScene::onTouchBegan, this);
	mouseListener->onTouchMoved = CC_CALLBACK_2(GamingScene::onTouchMoved, this);
	mouseListener->onTouchEnded = CC_CALLBACK_2(GamingScene::onTouchEnded, this);
	mouseListener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	auto mouseMoveEvent = EventListenerMouse::create();
	mouseMoveEvent->onMouseMove = [&](Event* event) 
	{
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
		_mousePosition = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mouseMoveEvent, 1);


	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(GamingScene::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardListener, this);


	return true;
}

void GamingScene::update(float f)
{
	mapScroll();
}

void GamingScene::mapScroll()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto mapPosition = _tiledMap->getPosition();

	//the scroll distance times in direction x
	int xMove = 0;
	//the scroll distance times in direction y
	int yMove = 0;


	Vec2 scroll(0, 0);

	//choose scroll speed with _mousePosition
	xMove = (origin.x + visibleSize.width - BOX_EDGE_WIDTH_SMALL < _mousePosition.x)
		+ (origin.x + visibleSize.width - BOX_EDGE_WIDTH < _mousePosition.x)
		- (origin.x + BOX_EDGE_WIDTH_SMALL > _mousePosition.x)
		- (origin.x + BOX_EDGE_WIDTH > _mousePosition.x);
	yMove = (origin.y + visibleSize.height - BOX_EDGE_HEIGHT_SMALL < _mousePosition.y)
		+ (origin.y + visibleSize.height - BOX_EDGE_HEIGHT < _mousePosition.y)
		- (origin.y + BOX_EDGE_HEIGHT_SMALL > _mousePosition.y)
		- (origin.y + BOX_EDGE_HEIGHT > _mousePosition.y);

	//set the value of scroll
	scroll += Vec2(-SCROLL_DISTANCE, 0) * xMove;
	scroll += Vec2(0, -SCROLL_DISTANCE) * yMove;

	//restrict screen view in the map
	if (_tiledMap->getBoundingBox().containsPoint((-scroll) + visibleSize) 
		&& (_tiledMap->getBoundingBox().containsPoint(-scroll))) {
		mapPosition += scroll;
		_tiledMap->setPosition(mapPosition);
	}


}

void GamingScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event) 
{
	auto mapPosition = _tiledMap->getPosition();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//W-UP, S-DOWN, A-LEFT, D-RIGHT
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(0, 50) + visibleSize)) {
			mapPosition += Vec2(0, -50);
			_tiledMap->setPosition(mapPosition);
		}
		break;
	case EventKeyboard::KeyCode::KEY_A:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(-50, 0))) {
			mapPosition += Vec2(50, 0);
			_tiledMap->setPosition(mapPosition);
		}
		break;
	case EventKeyboard::KeyCode::KEY_S:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(0, -50))) {
			mapPosition += Vec2(0, 50);
			_tiledMap->setPosition(mapPosition);
		}
		break;
	case EventKeyboard::KeyCode::KEY_D:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(50, 0) + visibleSize)) {
			mapPosition += Vec2(-50, 0);
			_tiledMap->setPosition(mapPosition);
		}
		break;
	/*case EventKeyboard::KeyCode::KEY_SPACE:
		focusOnBase();*/

	default:
		break;

	}
}

bool GamingScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point beganPoint = touch->getLocation();

	_mouseRect->setTouchStartToGL(beganPoint);
	_mouseRect->setTouchEndToGL(beganPoint);
	_mouseRect->setTouchStartToMap(_tiledMap->getPosition());
	_mouseRect->schedule(schedule_selector(MouseRect::update));

	return true;
}

void GamingScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point touchMoved = touch->getLocation();

	_mouseRect->setTouchEndToGL(touchMoved);
	_mouseRect->clear();
	_mouseRect->setVisible(true);

}

void GamingScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	_mouseRect->reset();
	/*Point touchEnded = touch->getLocation();

	//contain buildings in the mouse renctangle

	_mouseRect->setVisible(false);
	if (_mouseRect->isScheduled(schedule_selector(MouseRect::update))) {
		unschedule(schedule_selector(MouseRect::update));
	}
	_mouseRect->setTouchEndToMap(_tiledMap->getPosition());*/

}