#include "SceneGame.h"
#include "CarromConfig.h"
#include "CompDisk.h"
#include "CompStriker.h"
#include "CompHole.h"
#include "CompWall.h"

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

	// Physics world setup
	this->getPhysicsWorld()->setSubsteps(CARROM_PARAM_PHYSICS_SUB_STEPS);

	// Create the game table
	auto table = Sprite::createWithSpriteFrameName("Table.png");
	if (nullptr == table)
		return false;
	table->setPosition(center);
	this->addChild(table, CARROM_Z_LAYER_BACKGROUND);

	auto tableSize = table->getContentSize();

	// Create the table edges (walls)
	auto topEdge = Sprite::create("sprites/TableTopEdge.png");
	if (nullptr == topEdge)
		return false;
	auto wall = Wall::create();
	topEdge->addComponent(wall);
	wall->setup();
	topEdge->setAnchorPoint(Vec2(0.5, 0.0));
	topEdge->setPosition(Vec2(center.x, center.y + tableSize.height / 2));
	this->addChild(topEdge, CARROM_Z_LAYER_BACKGROUND);

	auto botEdge = Sprite::create("sprites/TableBotEdge.png");
	if (nullptr == botEdge)
		return false;
	wall = Wall::create();
	botEdge->addComponent(wall);
	wall->setup();
	botEdge->setAnchorPoint(Vec2(0.5, 1.0));
	botEdge->setPosition(Vec2(center.x, center.y - tableSize.height / 2));
	this->addChild(botEdge, CARROM_Z_LAYER_BACKGROUND);

	auto rightEdge = Sprite::create("sprites/TableRightEdge.png");
	if (nullptr == rightEdge)
		return false;
	wall = Wall::create();
	rightEdge->addComponent(wall);
	wall->setup();
	rightEdge->setAnchorPoint(Vec2(0.0, 0.5));
	rightEdge->setPosition(Vec2(center.x + tableSize.width / 2, center.y));
	this->addChild(rightEdge, CARROM_Z_LAYER_BACKGROUND);

	auto leftEdge = Sprite::create("sprites/TableLeftEdge.png");
	if (nullptr == leftEdge)
		return false;
	wall = Wall::create();
	leftEdge->addComponent(wall);
	wall->setup();
	leftEdge->setAnchorPoint(Vec2(1.0, 0.5));
	leftEdge->setPosition(Vec2(center.x - tableSize.width / 2, center.y));
	this->addChild(leftEdge, CARROM_Z_LAYER_BACKGROUND);

	// Create a physics material for Striker and Disks
	auto physicsMaterial = PhysicsMaterial(0.1f, 0.75f, 0.5f);

	// Create and position the Striker
	auto strikerSprite = Sprite::createWithSpriteFrameName("DiskStriker.png");
	auto striker = Striker::create();
	strikerSprite->addComponent(striker);
	striker->setup(physicsMaterial, CARROM_PARAM_DAMPING);
	strikerSprite->setPosition(Vec2(center.x, center.y - 2.5f * tableSize.height / 7));
	this->addChild(strikerSprite, CARROM_Z_LAYER_GAMEPLAY);

	// Create and position the Red disk
	auto spriteName = "DiskRed.png";
	auto diskSprite = Sprite::createWithSpriteFrameName(spriteName);
	auto disk = Disk::create();
	diskSprite->addComponent(disk);
	disk->setup(physicsMaterial, CARROM_PARAM_DAMPING);
	diskSprite->setPosition(center);
	this->addChild(diskSprite, CARROM_Z_LAYER_GAMEPLAY);

	// Create and position Black and White disks
	float width = diskSprite->getContentSize().width;
	const float sqrt3 = 1.7320508075688772935f;

	float offsetAngle = CARROM_PI / 6;
	for (int r = 1; r <= 2; ++r)
	{
		float deltaAngle = (2 * CARROM_PI) / (6 * r);
		float radius = width;
		for (int i = 0; i < 6 * r; ++i)
		{
			if (i % 2 == 0)
			{
				spriteName = "DiskWhite.png";
				radius = width * r;
			}
			else
			{
				spriteName = "DiskBlack.png";
				radius = width * (r == 1 ? r : sqrt3);
			}

			auto diskSprite = Sprite::createWithSpriteFrameName(spriteName);
			auto disk = Disk::create();
			diskSprite->addComponent(disk);
			disk->setup(physicsMaterial, CARROM_PARAM_DAMPING);

			float angle = deltaAngle * i + offsetAngle;
			auto pos = center + Vec2(radius * cos(angle), radius * sin(angle));
			diskSprite->setPosition(pos);

			this->addChild(diskSprite, CARROM_Z_LAYER_GAMEPLAY);
		}
	}

	// Create and position 4 table holes
	Vec2 offsetPosition = Vec2(1, -1);
	for (int i = 0; i < 4; i++)
	{
		auto holeSprite = Sprite::createWithSpriteFrameName("Hole.png");
		auto hole = Hole::create();
		holeSprite->addComponent(hole);
		hole->setup();

		offsetPosition.x *= powf(-1, i + 1);
		offsetPosition.y *= powf(-1, i);
		holeSprite->setRotation(i * 90);
		holeSprite->setPosition(Vec2(
			center.x + offsetPosition.x * tableSize.width / 2,
			center.y + offsetPosition.y * tableSize.height / 2));

		this->addChild(holeSprite, CARROM_Z_LAYER_BACKGROUND + 1);
	}

	// Add keyboard listener for android back button
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void Game::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		// Back to the main menu
		Director::getInstance()->popScene();
	}
}