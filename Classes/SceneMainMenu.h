#ifndef __CARROM_SCENE_MAINMENU_H__
#define __CARROM_SCENE_MAINMENU_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);
	void startButtonCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(MainMenu);
};

#endif // __CARROM_SCENE_MAINMENU_H__
