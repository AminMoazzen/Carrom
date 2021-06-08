#ifndef __CARROM_COMP_DISK_H__
#define __CARROM_COMP_DISK_H__

#include "cocos2d.h"

USING_NS_CC;

using namespace experimental;

class Disk : public Component
{
public:
	static Disk* create()
	{
		Disk* disk = new Disk();
		if (disk && disk->init())
		{
			disk->autorelease();
			return disk;
		}
		CC_SAFE_DELETE(disk);
		return NULL;
	};

	bool setup(PhysicsMaterial& material, float damping);

private:
	bool init();
};

#endif // __CARROM_COMP_DISK_H__
