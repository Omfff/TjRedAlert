#ifndef __MOUNSE_RECT_H__
#define __MOUNSE_RECT_H__

#include"cocos2d.h"

class MouseRect :public cocos2d::DrawNode
{
public:

	CREATE_FUNC(MouseRect);

	//set _touchStartToGL with point
	void setTouchStartToGL(cocos2d::Point point);

	//set _touchEndToGL with point
	void setTouchEndToGL(cocos2d::Point point);

	//covert _touchStartToGL to _touchStartToMap by passing the mapPonit
	void setTouchStartToMap(cocos2d::Point mapPoint);

	//covert _touchEndToGL to _touchEndToMap by passing the mapPonit
	void setTouchEndToMap(cocos2d::Point mapPoint);

	//update mouse renctangle by changing touchEndToMap
	void update(float f) override;

	//stop update and clear 
	void reset();

private:

	cocos2d::Point _touchStartToGL;
	cocos2d::Point _touchEndToGL;

	//start position to tiledmap
	cocos2d::Point _touchStartToMap;
	//end position to tiledmap
	cocos2d::Point _touchEndToMap;
	
};
#endif // !__MOUNSE_RECT_H__

