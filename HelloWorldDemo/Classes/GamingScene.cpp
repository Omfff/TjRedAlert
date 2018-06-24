
#include"GamingScene.h"

const string CHOOSE_RESOURCES[8][5] =
{ { "", "units/Base(red).png", "units/Base(blue).png", "units/Base(green).png", "units/Base(yellow).png" },
{ "", "units/PowerPlant(red).png", "units/PowerPlant(blue).png", "units/PowerPlant(green).png", "units/PowerPlant(yellow).png" },
{ "", "units/Barrack(red).png", "units/Barrack(blue).png", "units/Barrack(green).png", "units/Barrack(yellow).png" },
{ "", "units/WarFactory(red).png", "units/WarFactory(blue).png", "units/WarFactory(green).png", "units/WarFactory(yellow).png" },
{ "", "units/OreRefinery(red).png", "units/OreRefinery(blue).png", "units/OreRefinery(green).png", "units/OreRefinery(yellow).png" },
{ "", "units/GI(red).png", "units/GI(blue).png", "units/GI(green).png", "units/GI(yellow).png" },
{ "", "units/AttackDog(red).png", "units/AttackDog(blue).png", "units/AttackDog(green).png", "units/AttackDog(yellow).png" },
{ "", "units/Tank(red).png", "units/Tank(blue).png", "units/Tank(green).png", "units/Tank(yellow).png" } };

USING_NS_CC;

Scene* GamingScene::createScene(socket_server * socketServer, boost::shared_ptr<talk_to_server> socketClient)
{
	auto scene = Scene::create();
	auto layer = GamingScene::create(socketServer, socketClient);
	scene->addChild(layer);
	return scene;
}

GamingScene * GamingScene::create(socket_server * socketServer, boost::shared_ptr<talk_to_server> socketClient)
{
	GamingScene *gamingScene = new (std::nothrow) GamingScene();
	if (gamingScene && gamingScene->init(socketServer, socketClient))
	{
		gamingScene->autorelease();
		return gamingScene;
	}
	CC_SAFE_DELETE(gamingScene);

	return nullptr;
}

bool GamingScene::init(socket_server * socketServer, boost::shared_ptr<talk_to_server> socketClient)
{
	if (!Layer::create()) 
	{
		return false;
	}

	_socketServer = socketServer;
	_socketClient = socketClient;
	//_socketServer->button_start();
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	_tiledMap = TMXTiledMap::create("maps/TjRedAlertMap(1).tmx");
	_tiledMap->setAnchorPoint(Vec2(0, 0));
	//set map position according to camp
	switch (CampTypes(_socketClient->getCamp()))
	{
	case RED:
		_tiledMap->setPosition(Point(0, 0));
		break;
	case BLUE:
		_tiledMap->setPosition(Point(visibleSize.width - _tiledMap->getContentSize().width, visibleSize.height - _tiledMap->getContentSize().height));
		break;
	case GREEN:
		_tiledMap->setPosition(Point(0, visibleSize.height - _tiledMap->getContentSize().height));
		break;
	case YELLOW:
		_tiledMap->setPosition(Point(visibleSize.width - _tiledMap->getContentSize().width, 0));
		break;
	default:
		break;
	}
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
	_unitManager->setPlayerCamp(CampTypes(_socketClient->getCamp()));
	_unitManager->setGridmap(_gridMap);
	_unitManager->setTileMap(_tiledMap);
	_unitManager->setNextId(CampTypes(_socketClient->getCamp()));
	_unitManager->_socketServer = _socketServer;
	_unitManager->_socketClient = _socketClient;
	_unitManager->setMyBaseId(0);

	addChild(_unitManager);

	_menuSpriteLayer = Layer::create();
	addChild(_menuSpriteLayer, 2);

	_manufactureMenu = ManufactureMenu::create();
	_manufactureMenu->setPosition(Vec2(origin.x + visibleSize.width - 20, origin.y + visibleSize.height / 2));

	


	_manufactureMenu->setBuildingCallBack([&](Ref*) 
	{
		unschedule(schedule_selector(GamingScene::updateFightUnitWaitingNum));
		_menuSpriteLayer->removeAllChildren();

		

		auto base = Sprite::create(CHOOSE_RESOURCES[BASE][_socketClient->getCamp()]);
		base->setPosition(Director::getInstance()->getVisibleSize().width - 200, 
						  Director::getInstance()->getVisibleSize().height * 0.8);
		base->setScale(0.5);
		if (_unitManager->getMyBaseId()) 
		{
			base->setOpacity(100);
		}
		_menuSpriteLayer->addChild(base, 10, BASE_TAG);

		auto powerPlant = Sprite::create(CHOOSE_RESOURCES[POWERPLANT][_socketClient->getCamp()]);
		powerPlant->setPosition(Director::getInstance()->getVisibleSize().width - 200,
								Director::getInstance()->getVisibleSize().height * 0.7);
		powerPlant->setScale(0.5);
		if (!_unitManager->getMyBaseId()) 
		{
			powerPlant->setOpacity(100);
		}
		_menuSpriteLayer->addChild(powerPlant, 10, POWER_PLANT_TAG);

		auto barrack = Sprite::create(CHOOSE_RESOURCES[BARRACKS][_socketClient->getCamp()]);
		barrack->setPosition(Director::getInstance()->getVisibleSize().width - 200,
							 Director::getInstance()->getVisibleSize().height * 0.6);
		barrack->setScale(0.5);
		if (!_unitManager->getMyBaseId()) 
		{
			barrack->setOpacity(100);
		}
		_menuSpriteLayer->addChild(barrack, 10, BARRACK_TAG);

		auto warFactory = Sprite::create(CHOOSE_RESOURCES[WARFACTORY][_socketClient->getCamp()]);
		warFactory->setPosition(Director::getInstance()->getVisibleSize().width - 200,
								Director::getInstance()->getVisibleSize().height * 0.5);
		warFactory->setScale(0.5);
		if (!_unitManager->getMyBaseId()) 
		{
			warFactory->setOpacity(100);
		}
		_menuSpriteLayer->addChild(warFactory, 10, WAR_FACTORY_TAG);

		auto oreRefinery = Sprite::create(CHOOSE_RESOURCES[OREREFINERY][_socketClient->getCamp()]);
		oreRefinery->setPosition(Director::getInstance()->getVisibleSize().width - 200,
								 Director::getInstance()->getVisibleSize().height * 0.4);
		oreRefinery->setScale(0.5);
		if (!_unitManager->getMyBaseId()) 
		{
			oreRefinery->setOpacity(100);
		}
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
			if (rect.containsPoint(locationInNode) && target->getOpacity() == 255) 
			{
				target->setOpacity(50);
				target->setScale(1.0);
				if (!baseRect.containsPoint(baseLocation)) 
				{
					_menuSpriteLayer->removeChildByTag(BASE_TAG);
				}
				if (!powerPlantRect.containsPoint(powerPlantLocation)) 
				{
					_menuSpriteLayer->removeChildByTag(POWER_PLANT_TAG);
				}
				if (!barrackRect.containsPoint(barrackLocation)) 
				{
					_menuSpriteLayer->removeChildByTag(BARRACK_TAG);
				}
				if (!warFactoryRect.containsPoint(warFactoryLocation)) 
				{
					_menuSpriteLayer->removeChildByTag(WAR_FACTORY_TAG);
				}
				if (!oreRefineryRect.containsPoint(oreRefineryLocation)) 
				{
					_menuSpriteLayer->removeChildByTag(ORE_REFINERY_TAG);
				}
				return true;
			}
			if (!baseRect.containsPoint(baseLocation)
				&& !powerPlantRect.containsPoint(powerPlantLocation)
				&& !barrackRect.containsPoint(barrackLocation)
				&& !warFactoryRect.containsPoint(warFactoryLocation)
				&& !oreRefineryRect.containsPoint(oreRefineryLocation)) 
			{
				_menuSpriteLayer->removeAllChildren();
			}
			return false;
		};
		buildingAttachedToMouse->onTouchMoved = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			target->setScale(1);
			if (target->getOpacity() == 50) 
			{
				target->setPosition(_menuSpriteLayer->convertToNodeSpace(touch->getLocation()));
				Vec2 positionInMap = _tiledMap->convertToNodeSpace(touch->getLocation());
				Size size = target->getContentSize();
				Rect rect = Rect((positionInMap.x - size.width / 2) / 32.0, (positionInMap.y - size.height / 2) / 32.0, size.width / 32.0, size.height / 32.0);
				if (_gridMap->checkRectPosition(rect)) 
				{
					target->setColor(Color3B::RED);
				}
				else 
				{
					target->setColor(Color3B(255, 255, 255));
				}
			}

		};
		buildingAttachedToMouse->onTouchEnded = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			Vec2 positionInMap =_tiledMap->convertToNodeSpace( touch->getLocation());
			Size size = target->getContentSize();
			Rect rect = Rect((positionInMap.x - size.width / 2) / 32.0, (positionInMap.y - size.height / 2) / 32.0, size.width / 32.0, size.height / 32.0);

			UnitTypes unittype = UnitTypes(target->getTag() - 101);
			_menuSpriteLayer->removeChildByTag(target->getTag());
			
			if (!_gridMap->checkRectPosition(rect)) 
			{
				Vec2 correctPos = Vec2(positionInMap.x, positionInMap.y);
				_unitManager->creatProduceMessage(unittype, correctPos);
				SimpleAudioEngine::getInstance()->playEffect("Music/Build.wav");//“Ù–ß
				//auto tempUnit=_unitManager->creatUnit(_unitManager->getPlayerCamp(),unittype,positionInMap);
				if (unittype < 5)
				{
					_money->costMoney(COST[unittype]);
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
	_manufactureMenu->setArmyCallBack([&](Ref*) 
	{
		_menuSpriteLayer->removeAllChildren();

		auto soldier = Sprite::create(CHOOSE_RESOURCES[GI][_socketClient->getCamp()]);
		/*auto GIFrame = SpriteFrame::create(CHOOSE_RESOURCES[GI][_socketClient->getCamp()], Rect(0, 0, 35, 37));
		auto GI = Sprite::createWithSpriteFrame(GIFrame);*/
		soldier->setPosition(Director::getInstance()->getVisibleSize().width - 200,
						Director::getInstance()->getVisibleSize().height * 0.5);
		soldier->setScale(1.5);
		_menuSpriteLayer->addChild(soldier, 10, GI_TAG);

		_soliderWaitingNum = LabelBMFont::create();
		_soliderWaitingNum->initWithString(to_string(_unitManager->getWaitingGINum()), "fonts/MoneyFont.fnt", 50);
		_soliderWaitingNum->setPosition(Director::getInstance()->getVisibleSize().width - 200 + 42,
			Director::getInstance()->getVisibleSize().height * 0.5 + 5);
		_menuSpriteLayer->addChild(_soliderWaitingNum, 10);

		auto attackDog = Sprite::create(CHOOSE_RESOURCES[ATTACKDOG][_socketClient->getCamp()]);
		attackDog->setPosition(Director::getInstance()->getVisibleSize().width - 200,
							   Director::getInstance()->getVisibleSize().height * 0.4);
		attackDog->setScale(1.5);
		_menuSpriteLayer->addChild(attackDog, 10, ATTACK_DOG_TAG);

		_attackdogWaitingNum = LabelBMFont::create();
		_attackdogWaitingNum->initWithString(to_string(_unitManager->getWaitingAttackDogNum()), "fonts/MoneyFont.fnt", 50);
		_attackdogWaitingNum->setPosition(Director::getInstance()->getVisibleSize().width - 200 + 42,
			Director::getInstance()->getVisibleSize().height * 0.4 + 5);
		_menuSpriteLayer->addChild(_attackdogWaitingNum, 10);

		auto tank = Sprite::create(CHOOSE_RESOURCES[TANK][_socketClient->getCamp()]);
		tank->setPosition(Director::getInstance()->getVisibleSize().width - 200,
						  Director::getInstance()->getVisibleSize().height * 0.3);
		tank->setScale(1.5);
		_menuSpriteLayer->addChild(tank, 10, TANK_TAG);

		_tankWaitingNum = LabelBMFont::create();
		_tankWaitingNum->initWithString(to_string(_unitManager->getWaitingTankNum()), "fonts/MoneyFont.fnt", 50);
		_tankWaitingNum->setPosition(Director::getInstance()->getVisibleSize().width - 200 + 42,
			Director::getInstance()->getVisibleSize().height * 0.28 + 5);
		_menuSpriteLayer->addChild(_tankWaitingNum, 10);

		schedule(schedule_selector(GamingScene::updateArmyMenu));
		schedule(schedule_selector(GamingScene::updateFightUnitWaitingNum));

		auto armyAttatchedToMouse = EventListenerTouchOneByOne::create();
		armyAttatchedToMouse->onTouchBegan = [&](Touch* touch, Event* event)->bool
		{
			SimpleAudioEngine::getInstance()->playEffect("Music/Unit training.wav");//“Ù–ß

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

			if (rect.containsPoint(locationInNode)) 
			{
				return true;
			}
			if (!GIRect.containsPoint(GILocation)
				&& !attackDogRect.containsPoint(attackDogLocation)
				&& !tankRect.containsPoint(tankLocation)) 
			{
				unschedule(schedule_selector(GamingScene::updateFightUnitWaitingNum));
				_menuSpriteLayer->removeAllChildren();
			}
			return false;
		};
		
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
		
		armyAttatchedToMouse->setSwallowTouches(true);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(armyAttatchedToMouse, soldier);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(armyAttatchedToMouse->clone(), attackDog);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(armyAttatchedToMouse->clone(), tank);
	});

	addChild(_manufactureMenu, 10);



	auto moneyIcon = Sprite::create("ui/money/money.png");
	moneyIcon->setPosition(visibleSize.width - 180, 40);
	moneyIcon->setScale(0.75);
	addChild(moneyIcon, 1);

	_money = Money::create();
	_money->setAnchorPoint(Vec2(0.5, 0.5));
	_money->setPosition(visibleSize.width - 80, 40);
	_money->schedule(schedule_selector(Money::update));
	addChild(_money, 1);
	_unitManager->setmoney(_money);

	auto electricityIcon = Sprite::create("ui/electricity/Electricity.png");
	electricityIcon->setPosition(visibleSize.width - 180, 100);
	electricityIcon->setScale(0.75);
	addChild(electricityIcon, 1);
	
	_electricity = Electricity::create();
	_electricity->setAnchorPoint(Vec2(0.5, 0.5));
	_electricity->setPosition(visibleSize.width - 80, 100);
	_electricity->setunitManager(_unitManager);
	_electricity->setmoney(_money);
	_unitManager->setelectricity(_electricity);
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

	for (auto keyCode : _keyStatus) 
	{
		if (keyCode.second) 
		{
			keyPressedToMove(keyCode.first);
		}
	}
	
	//_unitManager->unitAttackUpdate();
	if(_electricity->powerOff==0)
		_unitManager->fighterUnitProductionUpdate(f);
	_unitManager->updateUnitState();
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
		&& (_tiledMap->getBoundingBox().containsPoint(-scroll))) 
	{
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
		if (_tiledMap->getContentSize().width < focusPosition.x + visibleSize.width) 
		{
			focusPosition.x = _tiledMap->getContentSize().width - visibleSize.width;
		}
		if (_tiledMap->getContentSize().height < focusPosition.y + visibleSize.height) 
		{
			focusPosition.y = _tiledMap->getContentSize().height - visibleSize.height;
		}
		if (focusPosition.x < 0) 
		{
			focusPosition.x = 0;
		}
		if (focusPosition.y < 0) 
		{
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
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(0, 5)))
		{
			_tiledMap->runAction(MoveBy::create(0.2f, Vec2(0, -5)));
			if (_tiledMap->getPosition().y <= visibleSize.height - size.height + 10) 
			{
			_tiledMap->stopAllActions();
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_A:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(-5, 0))) 
		{
			_tiledMap->runAction(MoveBy::create(0.2f, Vec2(5, 0)));
			if (_tiledMap->getPosition().x >= -10) 
			{
			_tiledMap->stopAllActions();
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_S:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(0, -5))) 
		{
			_tiledMap->runAction(MoveBy::create(0.2f, Vec2(0, 5)));
			if (_tiledMap->getPosition().y >= -10)
			{
			_tiledMap->stopAllActions();
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_D:
		if (_tiledMap->getBoundingBox().containsPoint(Vec2(5, 0))) 
		{
			_tiledMap->runAction(MoveBy::create(0.2f, Vec2(-5, 0)));
			if (_tiledMap->getPosition().x <= visibleSize.width - size.width + 10) 
			{
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
}

void GamingScene::updateBuildingMenu(float f)
{
	if (!_menuSpriteLayer->getChildrenCount()) 
	{
		unschedule(schedule_selector(GamingScene::updateBuildingMenu));
	}

	if (_menuSpriteLayer->getChildByTag(BASE_TAG) && !_unitManager->getMyBaseId())
	{
		if (!_money->checkMoney(COST[BASE])) 
		{
			_menuSpriteLayer->getChildByTag(BASE_TAG)->setOpacity(100);
		}
		else 
		{
			_menuSpriteLayer->getChildByTag(BASE_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(POWER_PLANT_TAG) && _unitManager->getMyBaseId()) 
	{
		if (!_money->checkMoney(COST[POWERPLANT])) 
		{
			_menuSpriteLayer->getChildByTag(POWER_PLANT_TAG)->setOpacity(100);
		}
		else
		{
			_menuSpriteLayer->getChildByTag(POWER_PLANT_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(BARRACK_TAG) && _unitManager->getMyBaseId()) 
	{
		if (!_money->checkMoney(COST[BARRACKS]) || !_electricity->checkElectricity(-POWER[BARRACKS])) 
		{
			_menuSpriteLayer->getChildByTag(BARRACK_TAG)->setOpacity(100);
		}
		else
		{
			_menuSpriteLayer->getChildByTag(BARRACK_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(WAR_FACTORY_TAG) && _unitManager->getMyBaseId()) 
	{
		if (!_money->checkMoney(2000) || !_electricity->checkElectricity(-POWER[WARFACTORY]))
		{
			_menuSpriteLayer->getChildByTag(WAR_FACTORY_TAG)->setOpacity(100);
		}
		else 
		{
			_menuSpriteLayer->getChildByTag(WAR_FACTORY_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(ORE_REFINERY_TAG) && _unitManager->getMyBaseId()) 
	{
		if (!_money->checkMoney(2000) || !_electricity->checkElectricity(-POWER[OREREFINERY])) 
		{
			_menuSpriteLayer->getChildByTag(ORE_REFINERY_TAG)->setOpacity(100);
		}
		else 
		{
			_menuSpriteLayer->getChildByTag(ORE_REFINERY_TAG)->setOpacity(255);
		}
	}

}

void GamingScene::updateArmyMenu(float f)
{
	if (!_menuSpriteLayer->getChildrenCount()) 
	{
		unschedule(schedule_selector(GamingScene::updateArmyMenu));
	}

	if (_menuSpriteLayer->getChildByTag(GI_TAG)) 
	{
		if (!_money->checkMoney(COST[GI])) 
		{
			_menuSpriteLayer->getChildByTag(GI_TAG)->setOpacity(100);
		}
		else 
		{
			_menuSpriteLayer->getChildByTag(GI_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(ATTACK_DOG_TAG)) 
	{
		if (!_money->checkMoney(COST[ATTACKDOG])) 
		{
			_menuSpriteLayer->getChildByTag(ATTACK_DOG_TAG)->setOpacity(100);
		} 
		else 
		{
			_menuSpriteLayer->getChildByTag(ATTACK_DOG_TAG)->setOpacity(255);
		}
	}
	if (_menuSpriteLayer->getChildByTag(TANK_TAG)) 
	{
		if (!_money->checkMoney(COST[TANK])) 
		{
			_menuSpriteLayer->getChildByTag(TANK_TAG)->setOpacity(100);
		}
		else 
		{
			_menuSpriteLayer->getChildByTag(TANK_TAG)->setOpacity(255);
		}
	}

}

void GamingScene::updateFightUnitWaitingNum(float dt)
{
	if (!_menuSpriteLayer->getChildrenCount())
	{
		unschedule(schedule_selector(GamingScene::updateFightUnitWaitingNum));
	}
	else
	{
		_soliderWaitingNum->setString(to_string(_unitManager->getWaitingGINum()));
		_tankWaitingNum->setString(to_string(_unitManager->getWaitingTankNum()));
		_attackdogWaitingNum->setString(to_string(_unitManager->getWaitingAttackDogNum()));
	}
	
}

