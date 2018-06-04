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
	_tiledMap->setAnchorPoint(Vec2(0, 0));
	addChild(_tiledMap, 0);

	_gridMap = new GridMap;
	setCollisionPos(_tiledMap, _gridMap);

	_msgs = new GameMessageGroup;

	_unitManager = UnitManager::create();
	_unitManager->setMessageGrop(_msgs);
	_unitManager->setPlayerCamp(RED);
	_unitManager->setGridmap(_gridMap);
	_unitManager->setTileMap(_tiledMap);
	_unitManager->setNextId(RED);
	addChild(_unitManager);

	_menuSpriteLayer = Layer::create();
	addChild(_menuSpriteLayer, 2);

	_manufactureMenu = ManufactureMenu::create();
	_manufactureMenu->setPosition(Vec2(origin.x + visibleSize.width - 20, origin.y + visibleSize.height / 2));

	_manufactureMenu->setBuildingCallBack([&](Ref*) {
		auto base = Sprite::create("units/Base.png");
		base->setPosition(100, 100);
		base->setScale(0.5);
		_menuSpriteLayer->addChild(base, 10, BASE_TAG);

		auto powerPlant = Sprite::create("units/PowerPlant.png");
		powerPlant->setPosition(200, 200);
		powerPlant->setScale(0.5);
		_menuSpriteLayer->addChild(powerPlant, 10, POWER_PLANT_TAG);

		auto barrack = Sprite::create("units/Barrack.png");
		barrack->setPosition(300, 300);
		barrack->setScale(0.5);
		_menuSpriteLayer->addChild(barrack, 10, BARRACK_TAG);

		auto warFactory = Sprite::create("units/WarFactory.png");
		warFactory->setPosition(400, 400);
		warFactory->setScale(0.5);
		_menuSpriteLayer->addChild(warFactory, 10, WAR_FACTORY_TAG);

		auto oreRefinery = Sprite::create("units/OreRefinery.png");
		oreRefinery->setPosition(500, 500);
		oreRefinery->setScale(0.5);
		_menuSpriteLayer->addChild(oreRefinery, 10, ORE_REFINERY_TAG);

		auto buildingAttachedToMouse = EventListenerTouchOneByOne::create();
		buildingAttachedToMouse->onTouchBegan = [&](Touch* touch, Event* event)->bool
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());

			Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size size = target->getContentSize();
			Rect rect = Rect(0, 0, size.width, size.height);

			Size baseSize = _menuSpriteLayer->getChildByTag(BASE_TAG)->getContentSize();
			Rect baseRect = Rect(0, 0, baseSize.width, baseSize.height);
			Vec2 baseLocation = _menuSpriteLayer->getChildByTag(BASE_TAG)->convertToNodeSpace(touch->getLocation());

			Size powerPlantSize = _menuSpriteLayer->getChildByTag(POWER_PLANT_TAG)->getContentSize();
			Rect powerPlantRect = Rect(0, 0, powerPlantSize.width, powerPlantSize.height);
			Vec2 powerPlantLocation = _menuSpriteLayer->getChildByTag(POWER_PLANT_TAG)->convertToNodeSpace(touch->getLocation());

			Size barrackSize = _menuSpriteLayer->getChildByTag(BARRACK_TAG)->getContentSize();
			Rect barrackRect = Rect(0, 0, barrackSize.width, barrackSize.height);
			Vec2 barrackLocation = _menuSpriteLayer->getChildByTag(BARRACK_TAG)->convertToNodeSpace(touch->getLocation());

			Size warFactorySize = _menuSpriteLayer->getChildByTag(WAR_FACTORY_TAG)->getContentSize();
			Rect warFactoryRect = Rect(0, 0, warFactorySize.width, warFactorySize.height);
			Vec2 warFactoryLocation = _menuSpriteLayer->getChildByTag(WAR_FACTORY_TAG)->convertToNodeSpace(touch->getLocation());

			Size oreRefinerySize = _menuSpriteLayer->getChildByTag(ORE_REFINERY_TAG)->getContentSize();
			Rect oreRefineryRect = Rect(0, 0, oreRefinerySize.width, oreRefinerySize.height);
			Vec2 oreRefineryLocation = _menuSpriteLayer->getChildByTag(ORE_REFINERY_TAG)->convertToNodeSpace(touch->getLocation());


			if (rect.containsPoint(locationInNode)) {
				target->setOpacity(50);
				target->setScale(1.0);
				if (!baseRect.containsPoint(baseLocation)) {
					_menuSpriteLayer->removeChildByTag(BASE_TAG);
				}
				if (!powerPlantRect.containsPoint(powerPlantLocation)) {
					_menuSpriteLayer->removeChildByTag(POWER_PLANT_TAG);
				}
				if (!barrackRect.containsPoint(barrackLocation)) {
					_menuSpriteLayer->removeChildByTag(BARRACK_TAG);
				}
				if (!warFactoryRect.containsPoint(warFactoryLocation)) {
					_menuSpriteLayer->removeChildByTag(WAR_FACTORY_TAG);
				}
				if (!oreRefineryRect.containsPoint(oreRefineryLocation)) {
					_menuSpriteLayer->removeChildByTag(ORE_REFINERY_TAG);
				}
				return true;
			}

			return false;
		};
		buildingAttachedToMouse->onTouchMoved = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			target->setPosition(_menuSpriteLayer->convertToNodeSpace(touch->getLocation()));
		};
		buildingAttachedToMouse->onTouchEnded = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			Vec2 positionInMap =_tiledMap->convertToNodeSpace( touch->getLocation());
			Size size = target->getContentSize();
			Rect rect = Rect(positionInMap.x/32.0, positionInMap.y/32.0, size.width / 32.0, size.height / 32.0);

			//target->setOpacity(255);
			UnitTypes unittype = UnitTypes(target->getTag() - 101);
			_menuSpriteLayer->removeChildByTag(target->getTag());
			
			if (!_gridMap->checkRectPosition(rect)) {

			_money->costMoney(2500);
			_electricity->addElectricity(50);

			_unitManager->creatProduceMessage(unittype, positionInMap);
			auto tempUnit=_unitManager->creatUnit(_unitManager->getPlayerCamp(),unittype,positionInMap);
			if (tempUnit->getUnitType() < 5)
			{
				_money->addMoneyInPeriod(MONEY_PRODUCE[tempUnit->getUnitType()]);
				if (POWER[tempUnit->getUnitType()] > 0)
					_electricity->addElectricity(POWER[tempUnit->getUnitType()]);
				else
					_electricity->costElectricity(-POWER[tempUnit->getUnitType()]);
			}
			
			}
		};
		buildingAttachedToMouse->setSwallowTouches(true);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse, base);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse->clone(), powerPlant);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse->clone(), barrack);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse->clone(), warFactory);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse->clone(), oreRefinery);

	});

	addChild(_manufactureMenu, 10);

	auto moneyIcon = Sprite::create("ui/money/gold.png");
	moneyIcon->setPosition(visibleSize.width - 140, 20);
	moneyIcon->setScale(0.04);
	addChild(moneyIcon, 1);

	_money = Money::create();
	_money->setPosition(visibleSize.width - 100, 20);
	_money->schedule(schedule_selector(Money::update));
	addChild(_money, 1);
	
	_electricity = Electricity::create();
	//_electricity->setPosition(visibleSize.width - 100, 80);
	_electricity->setPosition(visibleSize.width - 100, 80);
	_electricity->setScale(0.5);
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
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseMoveEvent, this);


	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(GamingScene::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardListener, this);


	return true;
}

void GamingScene::update(float f)
{
	mapScroll();
	//_unitManager->updateUnitState();
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
	Rect mRect = _mouseRect->getMouseRect();
	_mouseRect->reset();
	if (mRect.size.height == 0)
		_unitManager->choosePosOrUnit(mRect.origin);
	else
	{
		_unitManager->selectUnits(transferRectToGridRect(mRect));
		_mouseRect->reset();
	}
	/*Point touchEnded = touch->getLocation();

	//contain buildings in the mouse renctangle

	_mouseRect->setVisible(false);
	if (_mouseRect->isScheduled(schedule_selector(MouseRect::update))) {
		unschedule(schedule_selector(MouseRect::update));
	}
	_mouseRect->setTouchEndToMap(_tiledMap->getPosition());*/


}
