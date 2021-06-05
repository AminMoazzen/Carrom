#include "Hole.h"
#include "SimpleAudioEngine.h"
#include "CarromConfig.h"

USING_NS_CC;

bool Hole::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto center = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	std::string fileName = "Hole.png";

	//auto sprite = Sprite::create(fileName);
	this->setTexture(fileName);
	/*if (sprite == nullptr)
	{
		return false;
	}*/

	//sprite->setPosition(position);
	auto spriteSize = this->getContentSize();
	auto physicsBody = PhysicsBody::createCircle(spriteSize.width / 2,
		PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(CATEGORY_BITMASK_HOLE);
	physicsBody->setContactTestBitmask(CATEGORY_BITMASK_DISK);
	physicsBody->setCollisionBitmask(CATEGORY_BITMASK_NONE);

	this->setAnchorPoint(Vec2::ZERO);
	this->setTag(12);
	//this->setAnchorPoint(Vec2::ONE / 2);
	this->addComponent(physicsBody);
	this->setContentSize(spriteSize);
	//this->addChild(sprite, 0);

	//adds contact event listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Hole::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	schedule(CC_SCHEDULE_SELECTOR(Hole::tick), 0.3f);
	return true;
}

void Hole::tick(float dt)
{
	/*auto sprite1 = addSpriteAtPosition(Vec2(center.x + cocos2d::random(-300, 300),
		center.y + cocos2d::random(-300, 300)));
	auto physicsBody = sprite1->getPhysicsBody();
	physicsBody->setVelocity(Vec2(cocos2d::random(-500, 500), cocos2d::random(-500, 500)));
	physicsBody->setContactTestBitmask(0xFFFFFFFF);*/
}

bool Hole::onContactBegin(PhysicsContact& contact)
{
	//	//this->setScale(this->_scaleX * 1.1f);
	//	auto nodeA = contact.getShapeA()->getBody()->getNode();
	//	auto nodeB = contact.getShapeB()->getBody()->getNode();
	//
	//	if (nodeA && nodeB)
	//	{
	//		if (nodeA->getTag() == 10)
	//		{
	//			//this->setColor(Color3B(255, 0, 0));
	//			//nodeA->setScale(0);
	//			nodeA->removeFromParentAndCleanup(true);
	//		}
	//		if (nodeB->getTag() == 10)
	//		{
	//			//this->setColor(Color3B(255, 0, 0));
	//			//nodeB->setScale(0);
	//			nodeB->removeFromParentAndCleanup(true);
	//		}
	//	}
	//
	//	//bodies can collide
	return true;
}