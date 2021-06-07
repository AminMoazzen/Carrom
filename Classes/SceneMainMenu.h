#ifndef __CARROM_SCENE_MAINMENU_H__
#define __CARROM_SCENE_MAINMENU_H__

#include "cocos2d.h"
#include "CCVector.h"

USING_NS_CC;

class MainMenu : public Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(MainMenu);

private:
	int mBgMusicID = -1;
	int mDiskIndex = 0;
	std::vector <Sprite*> disks;

	void tick(float dt);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void startButtonCallback(cocos2d::Ref* pSender);
};

#endif // __CARROM_SCENE_MAINMENU_H__
