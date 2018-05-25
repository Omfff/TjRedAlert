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
	Node* parent = getParent();
	setTouchEndToMap(parent->getPosition());
	drawRect(_touchStartToMap, _touchEndToMap, cocos2d::Color4F(0, 1, 0, 1));
}

void MouseRect::reset()
{
	setVisible(false);
	if (isScheduled(schedule_selector(MouseRect::update))) {
		unschedule(schedule_selector(MouseRect::update));
	}
	setTouchStartToGL({ 0, 0 });
	setTouchEndToGL({ 0, 0 });
	setTouchStartToMap(_touchStartToGL);
	setTouchEndToMap(_touchEndToGL);

}