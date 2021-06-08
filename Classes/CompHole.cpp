#include "CompHole.h"
#include "CarromConfig.h"

USING_NS_CC;

bool Hole::init()
{
	return true;
}

bool Hole::setup()
{
	auto owner = getOwner();
	auto contentSize = owner->getContentSize();
	auto physicsBody = PhysicsBody::createCircle(
		contentSize.width / 2,
		PHYSICSBODY_MATERIAL_DEFAULT,
		Vec2(-contentSize.width / 2, -contentSize.height / 2));
	physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(CARROM_CATEGORY_BITMASK_HOLE);
	physicsBody->setContactTestBitmask(CARROM_CATEGORY_BITMASK_DISK);
	physicsBody->setCollisionBitmask(CARROM_CATEGORY_BITMASK_NONE);

	owner->setAnchorPoint(Vec2::ZERO);
	owner->setTag(CARROM_TAG_HOLE);
	owner->addComponent(physicsBody);

	return true;
}