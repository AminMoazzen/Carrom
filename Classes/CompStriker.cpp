#include "CompStriker.h"
#include "CarromConfig.h"
#include <limits.h>

USING_NS_CC;

bool Striker::init()
{
	return true;
}

bool Striker::setup(PhysicsMaterial& material, float damping)
{
	auto owner = getOwner();
	auto contentSize = owner->getContentSize();
	Vec2 center = Vec2(contentSize.width / 2, contentSize.height / 2);

	// Create guide tape game object
	auto guide = DrawNode::create();
	Color4F white(1, 1, 1, 0.25f);
	float guideWidth = contentSize.width;
	guide->drawSolidRect(Vec2(-guideWidth / 2, 0), Vec2(guideWidth / 2, 1), white);
	guide->setName("guide");
	guide->setPosition(center);
	guide->setVisible(false);
	owner->addChild(guide, owner->getZOrder() - 1);

	// Create the physic body
	auto physicsBody = PhysicsBody::createCircle(contentSize.width / 2, material);
	physicsBody->setRotationEnable(false);
	physicsBody->setLinearDamping(damping);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(CARROM_CATEGORY_BITMASK_STRIKER);
	physicsBody->setContactTestBitmask(CARROM_CATEGORY_BITMASK_DISK | CARROM_CATEGORY_BITMASK_WALL);

	owner->addComponent(physicsBody);
	owner->setTag(CARROM_TAG_STRIKER);

	// Implement the contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Striker::onContactBegin, this);
	owner->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, owner);

	// Implement the touch listener
	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		mInitialTouchPos = touch->getLocation();
		if (owner->getBoundingBox().containsPoint(mInitialTouchPos))
		{
			physicsBody->setVelocity(Vec2::ZERO);
			auto rect = owner->getChildByName("guide");
			rect->setVisible(true);
			rect->setScaleY(0.1f);
			mIsTouched = true;
		}
		return true;
	};

	touchListener->onTouchMoved = [=](Touch* touch, Event* event)
	{
		if (mIsTouched)
		{
			auto touchLocation = touch->getLocation();
			Vec2 inputDirection = mInitialTouchPos - touchLocation;

			float inputLength = clampf(inputDirection.getLength(),
				std::numeric_limits<float>::epsilon(),
				std::numeric_limits<float>::max());

			auto rect = owner->getChildByName("guide");
			rect->setScaleY((MIN(inputLength * sensitivity, CARROM_PARAM_STRIKER_MAX_POWER) / CARROM_PARAM_STRIKER_MAX_POWER)
				* contentSize.width * 5);
			Vec2 normalInputDir = inputDirection / inputLength;

			rect->setRotation(Vec2::angle(Vec2::UNIT_Y, normalInputDir)
				* (Vec2::dot(Vec2::UNIT_X, normalInputDir) < 0 ? -1 : 1)
				* CARROM_RAD_TO_DEG);
		}
	};

	touchListener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		if (mIsTouched)
		{
			auto touchLocation = touch->getLocation();
			Vec2 inputDirection = mInitialTouchPos - touchLocation;

			float inputLength = clampf(inputDirection.getLength(),
				std::numeric_limits<float>::epsilon(),
				std::numeric_limits<float>::max());

			Vec2 normalInput = inputDirection / inputLength;
			float finalPower = MIN(inputLength * sensitivity, CARROM_PARAM_STRIKER_MAX_POWER);
			physicsBody->setVelocity(normalInput * finalPower);
			owner->getChildByName("guide")->setVisible(false);
			mIsTouched = false;
		}
	};

	owner->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, owner);

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
		const static float sEstimatedMaxSpeedSq = CARROM_PARAM_STRIKER_MAX_POWER * CARROM_PARAM_STRIKER_MAX_POWER;
		// If a Hole gets in contact with a Disk
		if (nodeA->getTag() == CARROM_TAG_HOLE && nodeB->getTag() == CARROM_TAG_DISK)
		{
			Pocket(nodeB);
		}
		// If a Disk gets in contact with a Hole
		else if (nodeB->getTag() == CARROM_TAG_HOLE && nodeA->getTag() == CARROM_TAG_DISK)
		{
			Pocket(nodeA);
		}
		// If a Disk gets in contact with anything other than a Hole
		else if (nodeA->getTag() == CARROM_TAG_DISK || nodeB->getTag() == CARROM_TAG_DISK)
		{
			// length of the velocities are used to control the hit sound volume
			float lengthA = velocityA.getLengthSq();
			float lengthB = velocityB.getLengthSq();
			lengthA > lengthB ? PlayHitSound(lengthA / sEstimatedMaxSpeedSq) : PlayHitSound(lengthB / sEstimatedMaxSpeedSq);
		}

		// If a Wall gets in contact with the Striker
		if (nodeA->getTag() == CARROM_TAG_WALL && nodeB->getTag() == CARROM_TAG_STRIKER)
		{
			// length of the velocity is used to control the hit sound volume
			float lengthB = velocityB.getLengthSq();
			PlayHitSound(lengthB / sEstimatedMaxSpeedSq);
		}
		// If the Striker gets in contact with a Wall
		else if (nodeB->getTag() == CARROM_TAG_WALL && nodeA->getTag() == CARROM_TAG_STRIKER)
		{
			// length of the velocity is used to control the hit sound volume
			float lengthA = velocityA.getLengthSq();
			PlayHitSound(lengthA / sEstimatedMaxSpeedSq);
		}
	}

	return true;
}