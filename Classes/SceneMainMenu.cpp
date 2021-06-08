#include "SceneMainMenu.h"
#include "SceneGame.h"
#include "AudioEngine.h"
#include "CarromConfig.h"

USING_NS_CC;

using namespace experimental;

Scene* MainMenu::createScene()
{
	return MainMenu::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
}

bool MainMenu::init()
{
	if (!Scene::init())
		return false;

	// Cache the spritesheet
	auto spriteCache = SpriteFrameCache::getInstance();
	spriteCache->addSpriteFramesWithFile("sprites/Sprites.plist");

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	Vec2 center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	// Create title Label
	auto labelTitle = Label::createWithTTF("Carrom", "fonts/Marker Felt.ttf", 72);
	if (nullptr == labelTitle)
		problemLoading("fonts/Marker Felt.ttf");
	labelTitle->setPosition(Vec2(center.x, origin.y + visibleSize.height - labelTitle->getContentSize().height));
	this->addChild(labelTitle, CARROM_Z_LAYER_BACKGROUND + 1);

	// Create music credit Label
	auto labelMusic = Label::createWithTTF("Music: www.bensound.com", "fonts/Marker Felt.ttf", 36);
	labelMusic->setAnchorPoint(Vec2(1.0, 0.0));
	auto labelMusicSize = labelTitle->getContentSize();
	labelMusic->setPosition(Vec2(origin.x + visibleSize.width - labelMusicSize.height, labelMusicSize.height));
	this->addChild(labelMusic, CARROM_Z_LAYER_BACKGROUND + 1);

	// Create Practice button
	auto itemStart = MenuItemImage::create();
	auto normalSpriteFrame = spriteCache->getSpriteFrameByName("ButtonStartNormal.png");
	auto selectedSpriteFrame = spriteCache->getSpriteFrameByName("ButtonStartPressed.png");
	itemStart->setNormalSpriteFrame(normalSpriteFrame);
	itemStart->setSelectedSpriteFrame(selectedSpriteFrame);
	itemStart->setCallback(CC_CALLBACK_1(MainMenu::startButtonCallback, this));
	itemStart->setAnchorPoint(Vec2(0.5, 0.0));
	itemStart->setPosition(Vec2(center.x, 50));

	auto labelPractice = Label::createWithTTF("Practice", "fonts/Marker Felt.ttf", 60);
	labelPractice->setPosition(Vec2(itemStart->getContentSize().width / 2, itemStart->getContentSize().height / 2));
	itemStart->addChild(labelPractice, CARROM_Z_LAYER_BACKGROUND + 2);

	// Create menu
	auto menu = Menu::create(itemStart, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, CARROM_Z_LAYER_BACKGROUND + 1);

	// Create background image
	auto background = Sprite::create("sprites/MenuBackground.png");
	if (nullptr == background)
		problemLoading("sprites/MenuBackground.png");
	background->setPosition(center);
	background->setContentSize(Size(visibleSize.width * 2, visibleSize.height * 2));
	this->addChild(background, CARROM_Z_LAYER_BACKGROUND);

	// Create disks to animate
	disks.push_back(Sprite::createWithSpriteFrameName("DiskWhite.png"));
	disks.push_back(Sprite::createWithSpriteFrameName("DiskRed.png"));
	disks.push_back(Sprite::createWithSpriteFrameName("DiskBlack.png"));

	auto diskSize = disks[0]->getContentSize();
	int halfDisksSize = disks.size() / 2;
	for (int i = 0; i < disks.size(); ++i)
	{
		disks[i]->setPosition(Vec2(
			visibleSize.width / 2 + origin.x + 4 * (i - halfDisksSize) * diskSize.width,
			visibleSize.height / 2 + origin.y));
		disks[i]->setScale(2.0);
		this->addChild(disks[i], CARROM_Z_LAYER_BACKGROUND + 1);
	}

	// Add keyboard event listener for android back button callback
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(MainMenu::onKeyReleased, this);
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	// Create scheduler for main menu animation
	const float bgMusicTempo = 114.0f;
	schedule(CC_SCHEDULE_SELECTOR(MainMenu::tick), 60.0f / bgMusicTempo);

	Vec2 diskPos = disks[mDiskIndex]->getPosition();
	disks[mDiskIndex]->setPosition(Vec2(diskPos.x, diskPos.y + disks[mDiskIndex]->getContentSize().height));

	return true;
}

void MainMenu::tick(float dt)
{
	float moveDuration = dt / 2;
	auto currentDisk = disks[mDiskIndex];
	auto moveBy = MoveBy::create(moveDuration, Vec2(0, -currentDisk->getContentSize().height));
	currentDisk->runAction(moveBy);

	int prev = mDiskIndex;
	do
	{
		mDiskIndex = random(0, (int)disks.size() - 1);
	} while (prev == mDiskIndex);

	currentDisk = disks[mDiskIndex];
	moveBy = MoveBy::create(moveDuration, Vec2(0, currentDisk->getContentSize().height));
	currentDisk->runAction(moveBy);
}

void MainMenu::onEnter()
{
	Node::onEnter();
	mBgMusicID = AudioEngine::play2d("sounds/MusicMenu.mp3", true);
}

void MainMenu::onExit()
{
	Node::onExit();
	AudioEngine::stop(mBgMusicID);
}

void MainMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		//Close the cocos2d-x game scene and quit the application
		Director::getInstance()->end();

		/* To navigate back to native iOS screen(if present) without quitting the application,
		 * do not use Director::getInstance()->end() as given above, instead trigger a custom event
		 * created in RootViewController.mm as below
		*/

		//EventCustom customEndEvent("game_scene_close_event");
		//_eventDispatcher->dispatchEvent(&customEndEvent);
	}
}

void MainMenu::startButtonCallback(Ref* pSender)
{
	AudioEngine::play2d("sounds/SFXButton.mp3");

	auto game = Game::createScene();
	Director::getInstance()->pushScene(game);
}