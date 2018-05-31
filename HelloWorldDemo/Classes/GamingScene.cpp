#include"GamingScene.h"
#include"Buliding.h"
#include"unitManager.h"

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

	auto moneyIcon = Sprite::create("CloseNormal.png");
	moneyIcon->setPosition(visibleSize.width - 120, 20);
	moneyIcon->setScale(0.04);
	addChild(moneyIcon, 1);


	_gridMap = new GridMap;
	_msgs = new GameMessageGroup;
	_unitManager = UnitManager::create();	
	_unitManager->setMessageGrop(_msgs);
	_unitManager->setPlayerCamp(RED);
	_unitManager->setGridmap(_gridMap);
	_unitManager->setTileMap(_tiledMap);
	_unitManager->setNextId(RED);
	addChild(_unitManager);
	unit=_unitManager->creatUnit(RED, BASE, Vec2(visibleSize.width / 3, visibleSize.height / 3));
	unit->setPosition(Vec2(visibleSize.width / 3, visibleSize.height / 3));
	_tiledMap->addChild(unit, 10);

	_unitManager->creatProduceMessage(BASE, Vec2(visibleSize.width / 4, visibleSize.height / 3));
	_unitManager->updateUnitState();

	/*unit = Base::create("base_0.png");
	unit->
	bool init(int id, CampTypes camp, BuildingTypes buildingType, GridVec2 point, TMXTiledMap* map, GridMap *gridmap);
	unit->init(1, RED, BASE, Vec2(visibleSize.width / 64, visibleSize.height / 96), _tiledMap, _gridMap);
	*/
	/*_money = Money::create();
	_money->setPosition(visibleSize.width - 80, 20);
	_money->schedule(schedule_selector(Money::update));
	addChild(_money, 1);*/
	

	/*_electricity = Electricity::create();
	_electricity->setPosition(visibleSize.width - 80, 80);
	addChild(_electricity, 1);*/

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

	xMove = (origin.x + visibleSize.width - BOX_EDGE_WIDTH_SMALL < _mousePosition.x)
		+ (origin.x + visibleSize.width - BOX_EDGE_WIDTH < _mousePosition.x)
		- (origin.x + BOX_EDGE_WIDTH_SMALL > _mousePosition.x)
		- (origin.x + BOX_EDGE_WIDTH > _mousePosition.x);
	yMove = (origin.y + visibleSize.height - BOX_EDGE_HEIGHT_SMALL < _mousePosition.y)
		+ (origin.y + visibleSize.height - BOX_EDGE_HEIGHT < _mousePosition.y)
		- (origin.y + BOX_EDGE_HEIGHT_SMALL > _mousePosition.y)
		- (origin.y + BOX_EDGE_HEIGHT > _mousePosition.y);

	scroll += Vec2(-SCROLL_DISTANCE, 0) * xMove;
	scroll += Vec2(0, -SCROLL_DISTANCE) * yMove;

	if (_tiledMap->getBoundingBox().containsPoint((-scroll) + visibleSize) 
		&& (_tiledMap->getBoundingBox().containsPoint(-scroll))) {
		mapPosition += scroll;
		_tiledMap->setPosition(mapPosition);
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
	Rect mRect=_mouseRect->getMouseRect();
	_unitManager->selectUnits(transferRectToGridRect(mRect));
	_mouseRect->reset();
	/*Point touchEnded = touch->getLocation();

	//contain buildings in the mouse renctangle

	_mouseRect->setVisible(false);
	if (_mouseRect->isScheduled(schedule_selector(MouseRect::update))) {
		unschedule(schedule_selector(MouseRect::update));
	}
	_mouseRect->setTouchEndToMap(_tiledMap->getPosition());*/

}