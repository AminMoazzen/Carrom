#ifndef __CARROM_DISK_H__
#define __CARROM_DISK_H__

#include "cocos2d.h"

USING_NS_CC;

enum class DiskType
{
	White,
	Black,
	Red,
};

class Disk : public CCSprite
{
public:
	static Disk* create(DiskType type)
	{
		Disk* disk = new Disk();
		if (disk && disk->init(type))
		{
			disk->autorelease();
			return disk;
		}
		CC_SAFE_DELETE(disk);
		return NULL;
	};

	bool init(DiskType type);
	void tick(float dt);
	bool onContactBegin(PhysicsContact& contact);
};

#endif // __CARROM_DISK_H__
