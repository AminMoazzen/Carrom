#include "Striker.h"
#include "CarromConfig.h"

USING_NS_CC;

bool Striker::init()
{
	return true;
}

bool Striker::setup(PhysicsMaterial& material, float damping)
{
	auto owner = getOwner();

	auto physicsBody = PhysicsBody::createCircle(owner->getContentSize().width / 2, material);
	physicsBody->setRotationEnable(false);
	physicsBody->setLinearDamping(damping);
	physicsBody->setGravityEnable(false);
	physicsBody->setTag(CARROM_TAG_STRIKER);
	physicsBody->setCategoryBitmask(CARROM_CATEGORY_BITMASK_STRIKER);
	physicsBody->setContactTestBitmask(CARROM_CATEGORY_BITMASK_DISK | CARROM_CATEGORY_BITMASK_WALL);

	owner->addComponent(physicsBody);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Striker::onContactBegin, this);

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		auto touchLocation = touch->getLocation();
		if (owner->getBoundingBox().containsPoint(touchLocation))
		{
			physicsBody->setVelocity(Vec2::ZERO);
			mIsTouched = true;
		}
		return true;
	};

	touchListener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		if (mIsTouched)
		{
			auto touchLocation = touch->getLocation();
			Vec2 inputDirection = owner->getPosition() - touchLocation;
			float inputPower = inputDirection.getLength();
			Vec2 normalInput = inputDirection / inputPower;
			float finalPower = clampf(inputPower * power, 0, CARROM_PARAM_STRIKER_MAX_POWER);
			physicsBody->setVelocity(normalInput * finalPower);
			mIsTouched = false;
		}
	};
	owner->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, owner);
	owner->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, owner);

	return true;
}

bool Striker::onContactBegin(PhysicsContact& contact)
{
	auto bodyA = contact.getShapeA()->getBody();
	auto velocityA = bodyA->getVelocity();
	auto nodeA = bodyA->getNode();

	auto bodyB = contact.getShapeB()->getBody();
	auto velocityB = bodyB->getVelocity();
	auto nodeB = bodyB->getNode();

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == CARROM_TAG_HOLE && nodeB->getTag() == CARROM_TAG_DISK)
		{
			Pocket(nodeB);
		}
		else if (nodeB->getTag() == CARROM_TAG_HOLE && nodeA->getTag() == CARROM_TAG_DISK)
		{
			Pocket(nodeA);
		}
		else if (nodeA->getTag() == CARROM_TAG_DISK || nodeB->getTag() == CARROM_TAG_DISK)
		{
			float lengthA = velocityA.getLengthSq();
			float lengthB = velocityB.getLengthSq();
			float estimatedMax = CARROM_PARAM_STRIKER_MAX_POWER * CARROM_PARAM_STRIKER_MAX_POWER;
			lengthA > lengthB ? PlayHitSound(lengthA / estimatedMax) : PlayHitSound(lengthB / estimatedMax);
		}

		if (nodeA->getTag() == CARROM_TAG_WALL && nodeB->getTag() == CARROM_TAG_STRIKER)
		{
			float lengthB = velocityB.getLengthSq();
			float estimatedMax = CARROM_PARAM_STRIKER_MAX_POWER * CARROM_PARAM_STRIKER_MAX_POWER;
			PlayHitSound(lengthB / estimatedMax);
		}
		else if (nodeB->getTag() == CARROM_TAG_WALL && nodeA->getTag() == CARROM_TAG_STRIKER)
		{
			float lengthA = velocityA.getLengthSq();
			float estimatedMax = CARROM_PARAM_STRIKER_MAX_POWER * CARROM_PARAM_STRIKER_MAX_POWER;
			PlayHitSound(lengthA / estimatedMax);
		}
	}

	return true;
}