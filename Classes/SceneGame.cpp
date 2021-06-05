#define PI   3.1415926535897932385f

#include "SceneGame.h"
#include "SimpleAudioEngine.h"
#include "Disk.h"
#include "Striker.h"
#include "Hole.h"

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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	//Director::getInstance()->setClearColor(Color4F(83.0 / 255.0, 51.0 / 255.0, 8.0 / 255.0, 1));
	LayerColor* bgColor = LayerColor::create(Color4B(20, 10, 0, 255));
	bgColor->setAnchorPoint(origin);
	bgColor->setPosition(Vec2::ZERO);
	bgColor->setContentSize(Size(visibleSize.width * 2, visibleSize.height * 2));
	this->addChild(bgColor, -10);

	auto table = Sprite::create("Table.png");
	if (table == nullptr)
	{
		return false;
	}

	table->setPosition(center);
	this->addChild(table, 0);

	auto tableSize = table->getContentSize();
	

	auto topEdge = Sprite::create("TableTopEdge.png");
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

		topEdge->addComponent(physicsBody);
	}

	auto botEdge = Sprite::create("TableBotEdge.png");
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

		botEdge->addComponent(physicsBody);
	}

	auto rightEdge = Sprite::create("TableRightEdge.png");
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

		rightEdge->addComponent(physicsBody);
	}

	auto leftEdge = Sprite::create("TableLeftEdge.png");
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

void Game::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}