#include"MouseRect.h"

void MouseRect::setTouchStartToGL(cocos2d::Point point)
{
	_touchStartToGL = point;
}

void MouseRect::setTouchEndToGL(cocos2d::Point point)
{
	_touchEndToGL = point;
}

void MouseRect::setTouchStartToMap(cocos2d::Point mapPoint)
{
	_touchStartToMap = _touchStartToGL - mapPoint;
}

void MouseRect::setTouchEndToMap(cocos2d::Point mapPoint)
{
	_touchEndToMap = _touchEndToGL - mapPoint;
}

void MouseRect::update(float f)
{
	clear();

	//get map position to GL
	Node* parent = getParent();
	setTouchEndToMap(parent->getPosition());

	//drawRect with the position to map
	drawRect(_touchStartToMap, _touchEndToMap, cocos2d::Color4F(0, 1, 0, 1));
}

void MouseRect::reset()
{
	setVisible(false);

	//unschedule running update()
	if (isScheduled(schedule_selector(MouseRect::update))) {
		unschedule(schedule_selector(MouseRect::update));
	}

	//reset touch position
	setTouchStartToGL({ 0, 0 });
	setTouchEndToGL({ 0, 0 });

	//reset touch position to map 
	setTouchStartToMap(_touchStartToGL);
	setTouchEndToMap(_touchEndToGL);

}