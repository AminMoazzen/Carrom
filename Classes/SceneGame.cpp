#include "SceneGame.h"
#include "Disk.h"
#include "Striker.h"
#include "Hole.h"
#include "CarromConfig.h"

USING_NS_CC;

Scene* Game::createScene()
{
	return Game::create();
}

// on "init" you need to initialize your instance
bool Game::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	auto director = Director::getInstance();

	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	Vec2 center = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//director->setClearColor(Color4F(83.0 / 255.0, 51.0 / 255.0, 8.0 / 255.0, 1));
	LayerColor* bgColor = LayerColor::create(Color4B(20, 10, 0, 255));
	bgColor->setAnchorPoint(origin);
	bgColor->setPosition(Vec2::ZERO);
	bgColor->setContentSize(Size(visibleSize.width * 2, visibleSize.height * 2));
	this->addChild(bgColor, -10);

	auto table = Sprite::createWithSpriteFrameName("Table.png");
	if (table == nullptr)
	{
		return false;
	}

	table->setPosition(center);
	this->addChild(table, 0);

	auto tableSize = table->getContentSize();

	auto topEdge = Sprite::create("sprites/TableTopEdge.png");
	if (topEdge == nullptr)
	{
		return false;
	}
	else
	{
		topEdge->setAnchorPoint(Vec2(0.5, 0.0));
		topEdge->setPosition(Vec2(center.x, center.y + tableSize.height / 2));
		this->addChild(topEdge, 1);
		auto physicsBody = PhysicsBody::createBox(
			Size(topEdge->getContentSize().width, topEdge->getContentSize().height),
			PhysicsMaterial(0.1f, 1.0f, 1.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(CATEGORY_BITMASK_WALL);
		physicsBody->setContactTestBitmask(CATEGORY_BITMASK_DISK | CATEGORY_BITMASK_STRIKER);

		topEdge->addComponent(physicsBody);
	}

	auto botEdge = Sprite::create("sprites/TableBotEdge.png");
	if (botEdge == nullptr)
	{
		return false;
	}
	else
	{
		botEdge->setAnchorPoint(Vec2(0.5, 1.0));
		botEdge->setPosition(Vec2(center.x, center.y - tableSize.height / 2));
		this->addChild(botEdge, 1);
		auto physicsBody = PhysicsBody::createBox(
			Size(botEdge->getContentSize().width, botEdge->getContentSize().height),
			PhysicsMaterial(0.1f, 1.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(CATEGORY_BITMASK_WALL);
		physicsBody->setContactTestBitmask(CATEGORY_BITMASK_DISK | CATEGORY_BITMASK_STRIKER);

		botEdge->addComponent(physicsBody);
	}

	auto rightEdge = Sprite::create("sprites/TableRightEdge.png");
	if (rightEdge == nullptr)
	{
		return false;
	}
	else
	{
		rightEdge->setAnchorPoint(Vec2(0.0, 0.5));
		rightEdge->setPosition(Vec2(center.x + tableSize.width / 2, center.y));
		this->addChild(rightEdge, 1);
		auto physicsBody = PhysicsBody::createBox(
			Size(rightEdge->getContentSize().width, rightEdge->getContentSize().height),
			PhysicsMaterial(0.1f, 1.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(CATEGORY_BITMASK_WALL);
		physicsBody->setContactTestBitmask(CATEGORY_BITMASK_DISK | CATEGORY_BITMASK_STRIKER);

		rightEdge->addComponent(physicsBody);
	}

	auto leftEdge = Sprite::create("sprites/TableLeftEdge.png");
	if (leftEdge == nullptr)
	{
		return false;
	}
	else
	{
		leftEdge->setAnchorPoint(Vec2(1.0, 0.5));
		leftEdge->setPosition(Vec2(center.x - tableSize.width / 2, center.y));
		this->addChild(leftEdge, 1);
		auto physicsBody = PhysicsBody::createBox(
			Size(leftEdge->getContentSize().width, leftEdge->getContentSize().height),
			PhysicsMaterial(0.1f, 1.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(CATEGORY_BITMASK_WALL);
		physicsBody->setContactTestBitmask(CATEGORY_BITMASK_DISK | CATEGORY_BITMASK_STRIKER);

		leftEdge->addComponent(physicsBody);
	}

	auto striker = Striker::create();
	striker->setPosition(Vec2(center.x, center.y - 2.5f * tableSize.height / 7));
	this->addChild(striker, 2);

	auto type = DiskType::Red;
	auto disk = Disk::create(type);
	disk->setPosition(center);
	this->addChild(disk, 2);

	float width = disk->getContentSize().width;
	const float sqrt3 = 1.7320508075688772935f;

	float offsetAngle = PI / 6;
	for (int r = 1; r <= 2; ++r)
	{
		float deltaAngle = (2 * PI) / (6 * r);
		float radius = width;
		for (int i = 0; i < 6 * r; ++i)
		{
			if (i % 2 == 0)
			{
				type = DiskType::White;
				radius = width * r;
			}
			else
			{
				type = DiskType::Black;
				radius = width * (r == 1 ? r : sqrt3);
			}
			disk = Disk::create(type);
			float angle = deltaAngle * i + offsetAngle;
			auto pos = center + Vec2(radius * cos(angle), radius * sin(angle));
			disk->setPosition(pos);
			this->addChild(disk, 2);
		}
	}

	float margin = tableSize.width / 102.4f;

	{
		auto hole = Hole::create();
		hole->setPosition(Vec2(center.x - tableSize.width / 2, center.y - tableSize.height / 2));
		this->addChild(hole, 1);
	}

	{
		auto hole = Hole::create();
		hole->setRotation(270);
		hole->setPosition(Vec2(center.x + tableSize.width / 2, center.y - tableSize.height / 2));
		this->addChild(hole, 1);
	}

	{
		auto hole = Hole::create();
		hole->setRotation(180);
		hole->setPosition(Vec2(center.x + tableSize.width / 2, center.y + tableSize.height / 2));
		this->addChild(hole, 1);
	}

	{
		auto hole = Hole::create();
		hole->setRotation(90);
		hole->setPosition(Vec2(center.x - tableSize.width / 2, center.y + tableSize.height / 2));
		this->addChild(hole, 1);
	}

	return true;
}

void Game::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		//Director::getInstance()->end();
		Director::getInstance()->popScene();
	}
}