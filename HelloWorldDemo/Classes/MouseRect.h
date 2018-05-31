#ifndef __MOUNSE_RECT_H__
#define __MOUNSE_RECT_H__

#include"cocos2d.h"
#include"GridMap.h"
using namespace std;

class MouseRect :public cocos2d::DrawNode
{
public:
	CREATE_FUNC(MouseRect);

	void setTouchStartToGL(cocos2d::Point point);
	void setTouchEndToGL(cocos2d::Point point);
	void setTouchStartToMap(cocos2d::Point mapPoint);
	void setTouchEndToMap(cocos2d::Point mapPoint);
	Rect getMouseRect();
	//update mouse renctangle by changing touchEndToMap
	void update(float f) override;
	void reset();

private:
	cocos2d::Point _touchStartToGL;
	cocos2d::Point _touchEndToGL;
	cocos2d::Point _touchStartToMap;
	cocos2d::Point _touchEndToMap;
	
};
#endif // !__MOUNSE_RECT_H__

