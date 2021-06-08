#ifndef __CARROM_COMP_WALL_H__
#define __CARROM_COMP_WALL_H__

#include "cocos2d.h"

USING_NS_CC;

using namespace experimental;

class Wall : public Component
{
public:
	static Wall* create()
	{
		Wall* wall = new Wall();
		if (wall && wall->init())
		{
			wall->autorelease();
			return wall;
		}
		CC_SAFE_DELETE(wall);
		return NULL;
	};

	bool setup();

private:
	bool init();
};

#endif // __CARROM_COMP_WALL_H__
