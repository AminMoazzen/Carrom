#include "Disk.h"
#include "CarromConfig.h"
#include "AudioEngine.h"

USING_NS_CC;

using namespace experimental;

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
	this->setSpriteFrame(fileName);
	//auto sprite = Sprite::create(fileName);
	//if (sprite == nullptr)
	//{
	//	return false;
	//}

	//sprite->setPosition(position);

	auto physicsBody = PhysicsBody::createCircle(this->getContentSize().width / 2,
		PhysicsMaterial(0.1f, 0.75f, 1.0f));
	physicsBody->setRotationEnable(false);
	physicsBody->setLinearDamping(0.5);
	//physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(CATEGORY_BITMASK_DISK);
	physicsBody->setContactTestBitmask(
		CATEGORY_BITMASK_STRIKER
		| CATEGORY_BITMASK_DISK
		| CATEGORY_BITMASK_HOLE
		| CATEGORY_BITMASK_WALL);

	this->addComponent(physicsBody);
	this->setTag(TAG_DISK);
	//this->setContentSize(this->getContentSize());
	//this->addChild(sprite, 0);

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
	//this->setScale(this->_scaleX * 1.1f);
	auto bodyA = contact.getShapeA()->getBody();
	auto velocityA = bodyA->getVelocity();
	auto nodeA = bodyA->getNode();

	auto bodyB = contact.getShapeB()->getBody();
	auto velocityB = bodyB->getVelocity();
	auto nodeB = bodyB->getNode();

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == TAG_HOLE)
		{
			AudioEngine::play2d("sounds/SFXDiskPocket.mp3");
			nodeB->removeFromParentAndCleanup(true);
		}
		else if (nodeB->getTag() == TAG_HOLE)
		{
			AudioEngine::play2d("sounds/SFXDiskPocket.mp3");
			nodeA->removeFromParentAndCleanup(true);
		}
		else if (nodeA->getTag() == TAG_DISK || nodeB->getTag() == TAG_DISK)
		{
			float lengthA = velocityA.getLengthSq();
			float lengthB = velocityB.getLengthSq();
			float estimatedMax = 250000;
			float volume = lengthA > lengthB ? clampf(lengthA / estimatedMax, 0, 1) : clampf(lengthB / estimatedMax, 0, 1);
			AudioEngine::play2d("sounds/SFXDiskHit.mp3", false, volume);
		}
	}

	//bodies can collide
	return true;
}

void Disk::Destroy()
{
}