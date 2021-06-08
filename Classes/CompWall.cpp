#include "CompWall.h"
#include "CarromConfig.h"

USING_NS_CC;

bool Wall::init()
{
	return true;
}

bool Wall::setup()
{
	auto owner = getOwner();

	auto physicsBody = PhysicsBody::createBox(owner->getContentSize());
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(CARROM_CATEGORY_BITMASK_WALL);
	physicsBody->setContactTestBitmask(CARROM_CATEGORY_BITMASK_STRIKER | CARROM_CATEGORY_BITMASK_DISK);

	owner->addComponent(physicsBody);
	owner->setTag(CARROM_TAG_WALL);

	return true;
}