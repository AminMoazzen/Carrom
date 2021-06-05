#ifndef __CARROM_HOLE_H__
#define __CARROM_HOLE_H__

#include "cocos2d.h"

USING_NS_CC;

class Hole : public Sprite
{
public:
	static Hole* create()
	{
		Hole* disk = new Hole();
		if (disk && disk->init())
		{
			disk->autorelease();
			return disk;
		}
		CC_SAFE_DELETE(disk);
		return NULL;
	};

	bool init();
	void tick(float dt);
	bool onContactBegin(PhysicsContact& contact);
};

#endif // __CARROM_HOLE_H__
