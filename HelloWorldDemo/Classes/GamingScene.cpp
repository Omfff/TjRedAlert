
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

	_gridMap = GridMap::create();
	_gridMap->setContentSize(Size(4000, 1984));
	_gridMap->setPosition(0, 0);
	_tiledMap->addChild(_gridMap, 10);
	setCollisionPos(_tiledMap, _gridMap);
	_gridMap->setVisible(false);

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
		_menuSpriteLayer->removeAllChildren();

		auto base = Sprite::create("units/Base(red).png");
		base->setPosition(Director::getInstance()->getVisibleSize().width - 200, 
						  Director::getInstance()->getVisibleSize().height * 0.8);
		//base->setPosition(100, 100);
		base->setScale(1);
		//base->setAnchorPoint(Vec2(0, 0));
		_menuSpriteLayer->addChild(base, 10, BASE_TAG);
		if (_unitManager->getMyBaseId()) {
			base->setOpacity(100);
		}

		auto powerPlant = Sprite::create("units/PowerPlant(red).png");
		powerPlant->setPosition(Director::getInstance()->getVisibleSize().width - 200,
								Director::getInstance()->getVisibleSize().height * 0.7);
		//powerPlant->setPosition(200, 200);
		powerPlant->setScale(1);
		//powerPlant->setAnchorPoint(Vec2(0, 0));
		_menuSpriteLayer->addChild(powerPlant, 10, POWER_PLANT_TAG);

		auto barrack = Sprite::create("units/Barrack(red).png");
		barrack->setPosition(Director::getInstance()->getVisibleSize().width - 200,
							 Director::getInstance()->getVisibleSize().height * 0.6);
		//barrack->setPosition(300, 300);
		barrack->setScale(1);
		//barrack->setAnchorPoint(Vec2(0, 0));
		_menuSpriteLayer->addChild(barrack, 10, BARRACK_TAG);

		auto warFactory = Sprite::create("units/WarFactory(red).png");
		warFactory->setPosition(Director::getInstance()->getVisibleSize().width - 200,
								Director::getInstance()->getVisibleSize().height * 0.5);
		//warFactory->setPosition(400, 400);
		warFactory->setScale(1);
		//warFactory->setAnchorPoint(Vec2(0, 0));
		_menuSpriteLayer->addChild(warFactory, 10, WAR_FACTORY_TAG);

		auto oreRefinery = Sprite::create("units/OreRefinery(red).png");
		oreRefinery->setPosition(Director::getInstance()->getVisibleSize().width - 200,
								 Director::getInstance()->getVisibleSize().height * 0.4);
		//oreRefinery->setPosition(500, 500);
		oreRefinery->setScale(1);
		//oreRefinery->setAnchorPoint(Vec2(0, 0));
		_menuSpriteLayer->addChild(oreRefinery, 10, ORE_REFINERY_TAG);

		schedule(schedule_selector(GamingScene::updateBuildingMenu));

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

			unschedule(schedule_selector(GamingScene::updateBuildingMenu));
			if (rect.containsPoint(locationInNode) && target->getOpacity() == 255) {
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
			if (!baseRect.containsPoint(baseLocation)
				&& !powerPlantRect.containsPoint(powerPlantLocation)
				&& !barrackRect.containsPoint(barrackLocation)
				&& !warFactoryRect.containsPoint(warFactoryLocation)
				&& !oreRefineryRect.containsPoint(oreRefineryLocation)) {
				_menuSpriteLayer->removeAllChildren();
			}

			return false;
		};
		SimpleAudioEngine::getInstance()->playEffect("Music/Build.wav");//音效
		buildingAttachedToMouse->onTouchMoved = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			if (target->getOpacity() == 50) {
				target->setPosition(_menuSpriteLayer->convertToNodeSpace(touch->getLocation()));
				Vec2 positionInMap = _tiledMap->convertToNodeSpace(touch->getLocation());
				Size size = target->getContentSize();
				//Rect rect = Rect((positionInMap.x - size.width / 2) / 32.0, (positionInMap.y - size.height / 2) / 32.0, size.width / 32.0, size.height / 32.0);
				Rect rect = Rect(positionInMap.x / 32.0, positionInMap.y / 32.0, size.width / 32.0, size.height / 32.0);
				if (_gridMap->checkRectPosition(rect)) {
					target->setColor(Color3B::RED);
				}
				else {
					target->setColor(Color3B(255, 255, 255));
				}
			}

		};
		buildingAttachedToMouse->onTouchEnded = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			Vec2 positionInMap =_tiledMap->convertToNodeSpace( touch->getLocation());
			Size size = target->getContentSize();
			//Rect rect = Rect((positionInMap.x - size.width / 2) / 32.0, (positionInMap.y - size.height / 2) / 32.0, size.width / 32.0, size.height / 32.0);
			Rect rect = Rect(positionInMap.x / 32.0, positionInMap.y / 32.0, size.width / 32.0, size.height / 32.0);

			//target->setOpacity(255);
			UnitTypes unittype = UnitTypes(target->getTag() - 101);
			_menuSpriteLayer->removeChildByTag(target->getTag());
			
			if (!_gridMap->checkRectPosition(rect)) {
				_unitManager->creatProduceMessage(unittype, positionInMap);
				auto tempUnit=_unitManager->creatUnit(_unitManager->getPlayerCamp(),unittype,positionInMap);
				if (tempUnit->getUnitType() < 5)
				{
					_money->costMoney(COST[tempUnit->getUnitType()]);
					_money->addMoneyInPeriod(MONEY_PRODUCE[tempUnit->getUnitType()]);
					if (POWER[tempUnit->getUnitType()] > 0)
						_electricity->addElectricity(POWER[tempUnit->getUnitType()]);
					else
						_electricity->costElectricity(-POWER[tempUnit->getUnitType()]);
				}
			
			}
		};
		SimpleAudioEngine::getInstance()->playEffect("Music/Construction complete.wav");//音效
		buildingAttachedToMouse->setSwallowTouches(true);

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse, base);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse->clone(), powerPlant);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse->clone(), barrack);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse->clone(), warFactory);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(buildingAttachedToMouse->clone(), oreRefinery);
	});
	_manufactureMenu->setArmyCallBack([&](Ref*) {
		_menuSpriteLayer->removeAllChildren();

		auto GIFrame = SpriteFrame::create("units/GI(red).png", Rect(0, 0, 35, 37));
		auto GI = Sprite::createWithSpriteFrame(GIFrame);
		GI->setPosition(Director::getInstance()->getVisibleSize().width - 200,
						Director::getInstance()->getVisibleSize().height * 0.5);
		GI->setScale(2);
		_menuSpriteLayer->addChild(GI, 10, GI_TAG);

		auto attackDogFrame = SpriteFrame::create("units/AttackDog(red).png", Rect(0, 0, 49, 50));
		auto attackDog = Sprite::createWithSpriteFrame(attackDogFrame);
		attackDog->setPosition(Director::getInstance()->getVisibleSize().width - 200,
							   Director::getInstance()->getVisibleSize().height * 0.4);
		attackDog->setScale(1.4);
		_menuSpriteLayer->addChild(attackDog, 10, ATTACK_DOG_TAG);

		auto tankFrame = SpriteFrame::create("units/Tank(red).png", Rect(0, 0, 35, 37));
		auto tank = Sprite::Sprite::createWithSpriteFrame(tankFrame);
		tank->setPosition(Director::getInstance()->getVisibleSize().width - 200,
						  Director::getInstance()->getVisibleSize().height * 0.28);
		tank->setScale(2);
		_menuSpriteLayer->addChild(tank, 10, TANK_TAG);

		schedule(schedule_selector(GamingScene::updateArmyMenu));

		auto armyAttatchedToMouse = EventListenerTouchOneByOne::create();
		armyAttatchedToMouse->onTouchBegan = [&](Touch* touch, Event* event)->bool
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());

			Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size size = target->getContentSize();
			Rect rect = Rect(0, 0, size.width, size.height);

			Size GISize = _menuSpriteLayer->getChildByTag(GI_TAG)->getContentSize();
			Rect GIRect = Rect(0, 0, GISize.width, GISize.height);
			Vec2 GILocation = _menuSpriteLayer->getChildByTag(GI_TAG)->convertToNodeSpace(touch->getLocation());

			Size attackDogSize = _menuSpriteLayer->getChildByTag(ATTACK_DOG_TAG)->getContentSize();
			Rect attackDogRect = Rect(0, 0, attackDogSize.width, attackDogSize.height);
			Vec2 attackDogLocation = _menuSpriteLayer->getChildByTag(ATTACK_DOG_TAG)->convertToNodeSpace(touch->getLocation());

			Size tankSize = _menuSpriteLayer->getChildByTag(TANK_TAG)->getContentSize();
			Rect tankRect = Rect(0, 0, tankSize.width, tankSize.height);
			Vec2 tankLocation = _menuSpriteLayer->getChildByTag(TANK_TAG)->convertToNodeSpace(touch->getLocation());

			if (rect.containsPoint(locationInNode)) {
				target->setScale(2);
				return true;
			}
			if (!GIRect.containsPoint(GILocation)
				&& !attackDogRect.containsPoint(attackDogLocation)
				&& !tankRect.containsPoint(tankLocation)) {
				_menuSpriteLayer->removeAllChildren();
			}
			return false;
		};
		SimpleAudioEngine::getInstance()->playEffect("Music/Unit training.wav");//音效
		armyAttatchedToMouse->onTouchEnded = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			Vec2 positionInMap = _tiledMap->convertToNodeSpace(touch->getLocation());
			Size size = target->getContentSize();
			Rect rect = Rect(positionInMap.x / 32.0, positionInMap.y / 32.0, size.width / 32.0, size.height / 32.0);

			UnitTypes unittype = UnitTypes(target->getTag() - 101);

			if (_money->checkMoney(COST[unittype]))
			{
				_unitManager->_fighterProduceSeq.push_back(unittype);
				_money->costMoney(COST[unittype]);
				if (int(unittype) == 5)
					_unitManager->setWaitingGINum(_unitManager->getWaitingGINum() + 1);
				else if (int(unittype) == 6)
					_unitManager->setWaitingAttackDogNum(_unitManager->getWaitingAttackDogNum() + 1);
				else
					_unitManager->setWaitingTankNum(_unitManager->getWaitingTankNum() + 1);
			}

		};
		SimpleAudioEngine::getInstance()->playEffect("Music/Unit ready.wav");//音效
		armyAttatchedToMouse->setSwallowTouches(true);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(armyAttatchedToMouse, GI);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(armyAttatchedToMouse->clone(), attackDog);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(armyAttatchedToMouse->clone(), tank);
	});

	addChild(_manufactureMenu, 10);

	auto unit1 = _unitManager->creatUnit(RED, GI, Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_unitManager->creatProduceMessage(GI, Vec2(visibleSize.width / 2, visibleSize.height / 2));

	/*auto unit2 = _unitManager->creatUnit(RED, GI, Vec2(3 * visibleSize.width / 4, 3 * visibleSize.height / 4));
	_unitManager->creatProduceMessage(GI, Vec2(3 * visibleSize.width / 4, 3 * visibleSize.height / 4));*/

	auto moneyIcon = Sprite::create("ui/money/gold.png");
	moneyIcon->setPosition(visibleSize.width - 140, 20);
	moneyIcon->setScale(0.04);
	addChild(moneyIcon, 1);

	_money = Money::create();
	_money->setPosition(visibleSize.width - 80, 20);
	_money->schedule(schedule_selector(Money::update));
	addChild(_money, 1);
	
	_electricity = Electricity::create();
	_electricity->setPosition(visibleSize.width - 80, 60);
	_electricity->setScale(0.42);
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

	_keyStatus.insert(make_pair(EventKeyboard::KeyCode::KEY_W, false));
	_keyStatus.insert(make_pair(EventKeyboard::KeyCode::KEY_A, false));
	_keyStatus.insert(make_pair(EventKeyboard::KeyCode::KEY_S, false));
	_keyStatus.insert(make_pair(EventKeyboard::KeyCode::KEY_D, false));
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(GamingScene::onKeyPressed, this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(GamingScene::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
	


	return true;
}

void GamingScene::update(float f)
{
	mapScroll();

	for (auto keyCode : _keyStatus) {
		if (keyCode.second) {
			keyPressedToMove(keyCode.first);
		}
	}

	_unitManager->unitAttackUpdate();
	_unitManager->fighterUnitProductionUpdate();
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
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Point focusPosition = _unitManager->getMyBasePos();
	Size size = _tiledMap->getContentSize();

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_SPACE:
		focusPosition -= visibleSize / 2;
		if (_tiledMap->getContentSize().width < focusPosition.x + visibleSize.width) {
			focusPosition.x = _tiledMap->getContentSize().width - visibleSize.width;
		}
		if (_tiledMap->getContentSize().height < focusPosition.y + visibleSize.height) {
			focusPosition.y = _tiledMap->getContentSize().height - visibleSize.height;
		}
		if (focusPosition.x < 0) {
			focusPosition = 0;
		}
		if (focusPosition.y < 0) {
			focusPosition.y = 0;
		}
		focusPosition = Point(0, 0) - focusPosition;
		_tiledMap->setPosition(focusPosition);
		break;
	case EventKeyboard::KeyCode::KEY_W:
		_keyStatus[EventKeyboard::KeyCode::KEY_W] = true;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		_keyStatus[EventKeyboard::KeyCode::KEY_A] = true;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		_keyStatus[EventKeyboard::KeyCode::KEY_S] = true;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		_keyStatus[EventKeyboard::KeyCode::KEY_D] = true;
		break;
	default:
		break;
	}

}

void GamingScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	_keyStatus[keyCode] = false;
}

void GamingScene::keyPressedToMove(EventKeyboard::KeyCode keyCode)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Size size = _tiledMap->getContentSize();

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(0, 5))) {
			_tiledMap->runAction(MoveBy::create(0.2f, Vec2(0, -5)));
			if (_tiledMap->getPosition().y <= visibleSize.height - size.height + 10) {
			_tiledMap->stopAllActions();
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_A:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(-5, 0))) {
			_tiledMap->runAction(MoveBy::create(0.2f, Vec2(5, 0)));
			if (_tiledMap->getPosition().x >= -10) {
			_tiledMap->stopAllActions();
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_S:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(0, -5))) {
			_tiledMap->runAction(MoveBy::create(0.2f, Vec2(0, 5)));
			if (_tiledMap->getPosition().y >= -10) {
			_tiledMap->stopAllActions();
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_D:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(5, 0))) {
			_tiledMap->runAction(MoveBy::create(0.2f, Vec2(-5, 0)));
			if (_tiledMap->getPosition().x <= visibleSize.width - size.width + 10) {
			_tiledMap->stopAllActions();
			}
		}
		break;
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

void GamingScene::updateBuildingMenu(float f)
{
	if (!_menuSpriteLayer->getChildrenCount()) {
		unschedule(schedule_selector(GamingScene::updateBuildingMenu));
	}

	if (_menuSpriteLayer->getChildByTag(BASE_TAG) && !_unitManager->getMyBaseId()) {
		if (!_money->checkMoney(2500)) {
			_menuSpriteLayer->getChildByTag(BASE_TAG)->setOpacity(100);
		}
		else {
			_menuSpriteLayer->getChildByTag(BASE_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(POWER_PLANT_TAG)) {
		if (!_money->checkMoney(800)) {
			_menuSpriteLayer->getChildByTag(POWER_PLANT_TAG)->setOpacity(100);
		}
		else {
			_menuSpriteLayer->getChildByTag(POWER_PLANT_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(BARRACK_TAG)) {
		if (!_money->checkMoney(800) || !_electricity->checkElectricity(10)) {
			_menuSpriteLayer->getChildByTag(BARRACK_TAG)->setOpacity(100);
		}
		else {
			_menuSpriteLayer->getChildByTag(BARRACK_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(WAR_FACTORY_TAG)) {
		if (!_money->checkMoney(2000) || !_electricity->checkElectricity(50)) {
			_menuSpriteLayer->getChildByTag(WAR_FACTORY_TAG)->setOpacity(100);
		}
		else {
			_menuSpriteLayer->getChildByTag(WAR_FACTORY_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(ORE_REFINERY_TAG)) {
		if (!_money->checkMoney(2000) || !_electricity->checkElectricity(40)) {
			_menuSpriteLayer->getChildByTag(ORE_REFINERY_TAG)->setOpacity(100);
		}
		else {
			_menuSpriteLayer->getChildByTag(ORE_REFINERY_TAG)->setOpacity(255);
		}
	}

}

void GamingScene::updateArmyMenu(float f)
{
	if (_menuSpriteLayer->getChildrenCount()) {
		unschedule(schedule_selector(GamingScene::updateArmyMenu));
	}

	if (_menuSpriteLayer->getChildByTag(GI_TAG)) {
		if (!_money->checkMoney(150)) {
			_menuSpriteLayer->getChildByTag(GI_TAG)->setOpacity(100);
		}
		else {
			_menuSpriteLayer->getChildByTag(GI_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(ATTACK_DOG_TAG)) {
		if (!_money->checkMoney(150)) {
			_menuSpriteLayer->getChildByTag(ATTACK_DOG_TAG)->setOpacity(100);
		}
		else {
			_menuSpriteLayer->getChildByTag(ATTACK_DOG_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(TANK_TAG)) {
		if (!_money->checkMoney(300)) {
			_menuSpriteLayer->getChildByTag(TANK_TAG)->setOpacity(100);
		}
		else {
			_menuSpriteLayer->getChildByTag(TANK_TAG)->setOpacity(255);
		}
	}

}

