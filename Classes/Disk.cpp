#include "Disk.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

bool Disk::init(DiskType type)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto center = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	std::string fileName;// = "DiskWhite.png";
	switch (type)
	{
	case DiskType::White:
		fileName = "DiskWhite.png";
		break;
	case DiskType::Black:
		fileName = "DiskBlack.png";
		break;
	case DiskType::Red:
		fileName = "DiskRed.png";
		break;
	}
	auto sprite = Sprite::create(fileName);
	if (sprite == nullptr)
	{
		return false;
	}

	//sprite->setPosition(position);

	auto physicsBody = PhysicsBody::createCircle(sprite->getContentSize().width / 2,
		PhysicsMaterial(0.1f, 0.75f, 1.0f));
	physicsBody->setRotationEnable(false);
	physicsBody->setLinearDamping(0.5);
	//physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setTag(10);

	this->addComponent(physicsBody);
	this->setContentSize(sprite->getContentSize());
	this->addChild(sprite, 0);

	//adds contact event listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Disk::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	schedule(CC_SCHEDULE_SELECTOR(Disk::tick), 0.3f);
	return true;
}

void Disk::tick(float dt)
{
	/*auto sprite1 = addSpriteAtPosition(Vec2(center.x + cocos2d::random(-300, 300),
		center.y + cocos2d::random(-300, 300)));
	auto physicsBody = sprite1->getPhysicsBody();
	physicsBody->setVelocity(Vec2(cocos2d::random(-500, 500), cocos2d::random(-500, 500)));
	physicsBody->setContactTestBitmask(0xFFFFFFFF);*/
}

bool Disk::onContactBegin(PhysicsContact& contact)
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