#ifndef __CARROM_COMP_HOLE_H__
#define __CARROM_COMP_HOLE_H__

#include "cocos2d.h"

USING_NS_CC;

class Hole : public Component
{
public:
	static Hole* create()
	{
		Hole* hole = new Hole();
		if (hole && hole->init())
		{
			hole->autorelease();
			return hole;
		}
		CC_SAFE_DELETE(hole);
		return NULL;
	};

	bool setup();

private:
	bool init();
};

#endif // __CARROM_COMP_HOLE_H__
