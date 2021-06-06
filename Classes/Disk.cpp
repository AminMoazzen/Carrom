#include "Disk.h"
#include "CarromConfig.h"

USING_NS_CC;

bool Disk::init()
{
	return true;
}

bool Disk::setup(PhysicsMaterial& material, float damping)
{
	auto owner = getOwner();

	auto physicsBody = PhysicsBody::createCircle(owner->getContentSize().width / 2, material);
	physicsBody->setRotationEnable(false);
	physicsBody->setLinearDamping(damping);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(CARROM_CATEGORY_BITMASK_DISK);
	physicsBody->setContactTestBitmask(
		CARROM_CATEGORY_BITMASK_STRIKER
		| CARROM_CATEGORY_BITMASK_DISK
		| CARROM_CATEGORY_BITMASK_HOLE
		| CARROM_CATEGORY_BITMASK_WALL);

	owner->addComponent(physicsBody);
	owner->setTag(CARROM_TAG_DISK);

	return true;
}