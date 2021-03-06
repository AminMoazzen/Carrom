#ifndef __CARROM_SCENE_GAME_H__
#define __CARROM_SCENE_GAME_H__

#include "cocos2d.h"

USING_NS_CC;

class Game : public Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

private:
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif // __CARROM_SCENE_GAME_H__
