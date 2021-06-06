#include "Striker.h"
#include "CarromConfig.h"
#include "AudioEngine.h"

USING_NS_CC;

using namespace experimental;

bool Striker::init()
{
	//auto visibleSize = Director::getInstance()->getVisibleSize();

	std::string fileName = "DiskStriker.png";
	auto sprite = Sprite::createWithSpriteFrameName(fileName);
	if (sprite == nullptr)
	{
		return false;
	}

	auto spriteSize = sprite->getContentSize();
	Vec2 center = Vec2(spriteSize.width / 2, spriteSize.height / 2);
	//sprite->setPosition(position);

	auto physicsBody = PhysicsBody::createCircle(sprite->getContentSize().width / 2,
		PhysicsMaterial(0.1f, 0.75f, 1.0f));
	physicsBody->setRotationEnable(false);
	physicsBody->setLinearDamping(0.5);
	physicsBody->setGravityEnable(false);
	physicsBody->setTag(TAG_STRIKER);
	physicsBody->setCategoryBitmask(CATEGORY_BITMASK_STRIKER);
	physicsBody->setContactTestBitmask(CATEGORY_BITMASK_DISK | CATEGORY_BITMASK_WALL);

	this->addComponent(physicsBody);
	this->setContentSize(sprite->getContentSize());
	this->addChild(sprite, 0);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Striker::onContactBegin, this);

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		// your code
		auto touchLocation = touch->getLocation();
		if (this->getBoundingBox().containsPoint(touchLocation + center))
		{
			physicsBody->setVelocity(Vec2::ZERO);
			mIsTouched = true;
		}
		return true;
	};

	//touchListener->onTouchMoved = [](Touch* touch, Event* event)
	//{
	//};

	touchListener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		if (mIsTouched)
		{
			auto touchLocation = touch->getLocation();
			Vec2 inputDirection = this->getPosition() - touchLocation;
			float inputPower = inputDirection.getLength();
			Vec2 normalInput = inputDirection / inputPower;
			float finalPower = clampf(inputPower * power, 0, 500);
			physicsBody->setVelocity(normalInput * finalPower);
			mIsTouched = false;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	schedule(CC_SCHEDULE_SELECTOR(Striker::tick), 0.3f);
	return true;
}

void Striker::tick(float dt)
{
	/*auto sprite1 = addSpriteAtPosition(Vec2(center.x + cocos2d::random(-300, 300),
		center.y + cocos2d::random(-300, 300)));
	auto physicsBody = sprite1->getPhysicsBody();
	physicsBody->setVelocity(Vec2(cocos2d::random(-500, 500), cocos2d::random(-500, 500)));
	physicsBody->setContactTestBitmask(0xFFFFFFFF);*/
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
		if (nodeA->getTag() == TAG_WALL)
		{
			float lengthB = velocityB.getLengthSq();
			float estimatedMax = 250000;
			float volume = clampf(lengthB / estimatedMax, 0, 1);
			AudioEngine::play2d("sounds/SFXDiskHit.mp3", false, volume);
		}
		else if (nodeB->getTag() == TAG_WALL)
		{
			float lengthA = velocityA.getLengthSq();
			float estimatedMax = 250000;
			float volume = clampf(lengthA / estimatedMax, 0, 1);
			AudioEngine::play2d("sounds/SFXDiskHit.mp3", false, volume);
		}
	}

	return true;
}