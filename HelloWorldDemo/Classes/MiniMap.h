#pragma once

#include "cocos2d.h"
#include "SystemHeader.h"
#include "Unit.h"

USING_NS_CC;

class MiniMap : public cocos2d::Sprite
{
public:
	static MiniMap *create(const std::string &filename);
	void drawUnit(Unit * unit);
	float _miniMapWidth;
	float _miniMapHeight;
	DrawNode * _drawRectNode = nullptr;
	DrawNode * _drawDotNode = nullptr;
};
