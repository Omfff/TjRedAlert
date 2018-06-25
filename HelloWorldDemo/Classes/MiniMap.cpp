#include"MiniMap.h"

MiniMap * MiniMap::create(const std::string &filename) 
{
	MiniMap * ret = new MiniMap();
	if (ret && ret->initWithFile(filename)) 
	{
		ret->_drawRectNode = DrawNode::create();
		ret->addChild(ret->_drawRectNode);
		ret->_drawDotNode = DrawNode::create();
		ret->addChild(ret->_drawDotNode);
		ret->_miniMapHeight = ret->getBoundingBox().size.height;
		ret->_miniMapWidth = ret->getBoundingBox().size.height;
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void MiniMap::drawUnit(Unit * unit)
{
	switch (unit->getCamp()) 
	{
	case RED:
		_drawDotNode->drawDot(unit->getPosition() * 3 / 32, 4, Color4F::RED);
		break;
	case BLUE:
		_drawDotNode->drawDot(unit->getPosition() * 3 / 32, 4, Color4F::BLUE);
		break;
	case GREEN:
		_drawDotNode->drawDot(unit->getPosition() * 3 / 32, 4, Color4F::GREEN);
		break;
	case YELLOW:
		_drawDotNode->drawDot(unit->getPosition() * 3 / 32, 4, Color4F::YELLOW);
		break;
	default:
		break;
	}
}