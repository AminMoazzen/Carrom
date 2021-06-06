#ifndef __CARROM_SCENE_MAINMENU_H__
#define __CARROM_SCENE_MAINMENU_H__

#include "cocos2d.h"
#include "CCVector.h"

USING_NS_CC;

class MainMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);
	void startButtonCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(MainMenu);

private:
	int bgMusicID = -1;
	int index = 0;
	std::vector <Sprite*> disks;

	void tick(float dt);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif // __CARROM_SCENE_MAINMENU_H__
