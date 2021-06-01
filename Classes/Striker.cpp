#include "Striker.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

bool Striker::init()
{
	//auto visibleSize = Director::getInstance()->getVisibleSize();
	//auto center = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	std::string fileName = "DiskStriker.png";
	auto sprite = Sprite::create(fileName);
	if (sprite == nullptr)
	{
		return false;
	}

	//sprite->setPosition(position);

	auto physicsBody = PhysicsBody::createCircle(sprite->getContentSize().width / 2,
		PhysicsMaterial(0.1f, 0.75f, 1.0f));
	physicsBody->setRotationEnable(false);
	physicsBody->setLinearDamping(0.25);
	physicsBody->setGravityEnable(false);
	physicsBody->setTag(11);

	this->addComponent(physicsBody);
	this->setContentSize(sprite->getContentSize());
	this->addChild(sprite, 0);

	//adds contact event listener
	//auto contactListener = EventListenerPhysicsContact::create();
	//contactListener->onContactBegin = CC_CALLBACK_1(Striker::onContactBegin, this);
	auto touchListener = EventListenerTouchOneByOne::create();

	// trigger when you push down
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		// your code
		//currentTarget->;
		return true; // if you are consuming it
	};

	// trigger when moving touch
	touchListener->onTouchMoved = [](Touch* touch, Event* event) {
		// your code
	};

	// trigger when you let up
	touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
		auto touchLocation = touch->getLocation();
		auto currentTarget = event->getCurrentTarget();
		if (this->getBoundingBox().containsPoint(touchLocation))
			physicsBody->setVelocity(Vec2(0, 1000));
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

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
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == 10)
		{
			nodeB->removeFromParentAndCleanup(true);
		}
		else if (nodeB->getTag() == 10)
		{
			nodeA->removeFromParentAndCleanup(true);
		}
	}

	//bodies can collide
	return true;
}