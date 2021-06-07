#include "Striker.h"
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

	auto guide = DrawNode::create();
	Color4F white(1, 1, 1, 0.5f);
	guide->drawSolidRect(Vec2(-1, 0), Vec2(1, 2), white);
	guide->setName("guide");
	guide->setPosition(center);
	guide->setVisible(false);

	owner->addChild(guide, owner->getZOrder() - 1);

	auto physicsBody = PhysicsBody::createCircle(contentSize.width / 2, material);
	physicsBody->setRotationEnable(false);
	physicsBody->setLinearDamping(damping);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(CARROM_CATEGORY_BITMASK_STRIKER);
	physicsBody->setContactTestBitmask(CARROM_CATEGORY_BITMASK_DISK | CARROM_CATEGORY_BITMASK_WALL);

	owner->addComponent(physicsBody);
	owner->setTag(CARROM_TAG_STRIKER);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Striker::onContactBegin, this);

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		mInitialTouchPos = touch->getLocation();
		if (owner->getBoundingBox().containsPoint(mInitialTouchPos))
		{
			physicsBody->setVelocity(Vec2::ZERO);
			owner->getChildByName("guide")->setVisible(true);
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
			rect->setScaleY(MIN(inputLength * power, CARROM_PARAM_STRIKER_MAX_POWER) / 10);
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
			float finalPower = MIN(inputLength * power, CARROM_PARAM_STRIKER_MAX_POWER);
			physicsBody->setVelocity(normalInput * finalPower);
			owner->getChildByName("guide")->setVisible(false);
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