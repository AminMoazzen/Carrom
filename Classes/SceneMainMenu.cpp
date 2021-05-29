/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "SceneMainMenu.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

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
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	auto startItem = MenuItemImage::create(
		"ButtonStartNormal.png",
		"ButtonStartPressed.png",
		CC_CALLBACK_1(MainMenu::startButtonCallback, this));

	startItem->setAnchorPoint(cocos2d::Vec2(0.5, 0.0));

	if (startItem == nullptr ||
		startItem->getContentSize().width <= 0 ||
		startItem->getContentSize().height <= 0)
	{
		problemLoading("'ButtonStartNormal.png' and 'ButtonStartPressed.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width / 2;
		float y = 50;
		startItem->setPosition(Vec2(x, y));
	}

	auto labelStart = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 24);
	if (labelStart == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		//labelStart->setPosition(Vec2(origin.x + visibleSize.width / 2,
			//origin.y + visibleSize.height - label->getContentSize().height));

		float x = startItem->getContentSize().width / 2;
		float y = startItem->getContentSize().height / 2;
		labelStart->setPosition(Vec2(x, y));

		// add the label as a child to this layer
		startItem->addChild(labelStart, 1);
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, startItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Carrom", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	auto background = Sprite::create("MenuBackground.png");
	if (background == nullptr)
	{
		problemLoading("'MenuBackground.png'");
	}
	else
	{
		cocos2d::Texture2D::TexParams texParams = {
			GL_NEAREST,
			GL_NEAREST,
			GL_REPEAT,
			GL_REPEAT
		};
		background->getTexture()->setTexParameters(texParams);
		background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		background->setContentSize(Size(visibleSize.width * 2, visibleSize.height * 2));

		this->addChild(background, -10);
	}

	auto splash = Sprite::create("Splash.png");
	if (splash == nullptr)
	{
		problemLoading("'Splash.png'");
	}
	else
	{
		splash->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		this->addChild(splash, 0);
	}
	return true;
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}

void MainMenu::startButtonCallback(Ref* pSender)
{
}