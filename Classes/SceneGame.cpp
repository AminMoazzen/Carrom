#define PI   3.1415926535897932385f

#include "SceneGame.h"
#include "SimpleAudioEngine.h"
#include "Disk.h"
#include "Striker.h"

USING_NS_CC;

Scene* Game::createScene()
{
	return Game::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
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

	//auto closeItem = MenuItemImage::create(
	//	"CloseNormal.png",
	//	"CloseSelected.png",
	//	CC_CALLBACK_1(Game::menuCloseCallback, this));

	//if (closeItem == nullptr ||
	//	closeItem->getContentSize().width <= 0 ||
	//	closeItem->getContentSize().height <= 0)
	//{
	//	problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	//}
	//else
	//{
	//	float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
	//	float y = origin.y + closeItem->getContentSize().height / 2;
	//	closeItem->setPosition(Vec2(x, y));
	//}

	//// create menu, it's an autorelease object
	//auto menu = Menu::create(closeItem, NULL);
	//menu->setPosition(Vec2::ZERO);
	//this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...
	//Director::getInstance()->setClearColor(Color4F(83.0 / 255.0, 51.0 / 255.0, 8.0 / 255.0, 1));
	LayerColor* bgColor = LayerColor::create(Color4B(20, 10, 0, 255));
	bgColor->setAnchorPoint(origin);
	bgColor->setPosition(Vec2::ZERO);
	bgColor->setContentSize(Size(visibleSize.width * 2, visibleSize.height * 2));
	this->addChild(bgColor, -10);

	auto table = Sprite::create("Table.png");
	if (table == nullptr)
	{
		problemLoading("'Table.png'");
	}
	else
	{
		table->setPosition(center);

		this->addChild(table, 0);
	}

	//auto paw = Sprite::create("DiskPaw.png");
	//if (paw == nullptr)
	//{
	//	problemLoading("'DiskPaw.png'");
	//}
	//else
	//{
	//	paw->setPosition(Vec2(center.x, 0));
	//	this->addChild(paw, 1);
	//	auto physicsBody = PhysicsBody::createCircle(paw->getContentSize().width / 2,
	//		PhysicsMaterial(0.1f, 1.0f, 0.0f));
	//	//physicsBody->setDynamic(false);
	//	physicsBody->setRotationEnable(false);
	//	physicsBody->setGravityEnable(false);
	//	physicsBody->setLinearDamping(0.1);

	//	paw->addComponent(physicsBody);
	//}

	auto topEdge = Sprite::create("TableTopEdge.png");
	if (topEdge == nullptr)
	{
		problemLoading("'TableTopEdge.png'");
	}
	else
	{
		topEdge->setAnchorPoint(Vec2(0.5, 0.0));
		topEdge->setPosition(Vec2(center.x, center.y + table->getContentSize().height / 2));
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
		problemLoading("'TableBotEdge.png'");
	}
	else
	{
		botEdge->setAnchorPoint(Vec2(0.5, 1.0));
		botEdge->setPosition(Vec2(center.x, center.y - table->getContentSize().height / 2));
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
		problemLoading("'TableRightEdge.png'");
	}
	else
	{
		rightEdge->setAnchorPoint(Vec2(0.0, 0.5));
		rightEdge->setPosition(Vec2(center.x + table->getContentSize().width / 2, center.y));
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
		problemLoading("'TableLeftEdge.png'");
	}
	else
	{
		leftEdge->setAnchorPoint(Vec2(1.0, 0.5));
		leftEdge->setPosition(Vec2(center.x - table->getContentSize().width / 2, center.y));
		this->addChild(leftEdge, 1);
		auto physicsBody = PhysicsBody::createBox(
			Size(leftEdge->getContentSize().width, leftEdge->getContentSize().height),
			PhysicsMaterial(0.1f, 1.0f, 0.0f));
		physicsBody->setDynamic(false);

		leftEdge->addComponent(physicsBody);
	}

	//add five dynamic bodies
	//for (int i = 0; i < 5; ++i)
	//{
		//auto disk = Sprite::create("DiskWhite.png");
		//disk->setPosition(
		//	Vec2(center.x + cocos2d::random(-100, 100), center.y + cocos2d::random(-100, 100)));

		//auto physicsBody = PhysicsBody::createCircle(disk->getContentSize().width / 2,
		//	PhysicsMaterial(0.1f, 1.0f, 1.0f));
		//physicsBody->setRotationEnable(false);
		//physicsBody->setLinearDamping(0.1);

		//disk->addComponent(physicsBody);

		////set the body isn't affected by the physics world's gravitational force
		//physicsBody->setGravityEnable(false);

		////set initial velocity of physicsBody
		//physicsBody->setVelocity(Vec2(cocos2d::random(-500, 500),
		//	cocos2d::random(-500, 500)));
		//physicsBody->setTag(10);

		//this->addChild(disk, 1);
	//}

	auto striker = Striker::create();
	striker->setPosition(Vec2(center.x, center.y));
	this->addChild(striker, 1);

	auto type = DiskType::Red;
	auto disk = Disk::create(type);
	disk->setPosition(center);
	this->addChild(disk, 1);

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
			this->addChild(disk, 1);
		}
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