#ifndef __CARROM_STRIKER_H__
#define __CARROM_STRIKER_H__

#include "cocos2d.h"

USING_NS_CC;

class Striker : public Sprite
{
public:
	float power = 10;

	static Striker* create()
	{
		Striker* striker = new Striker();
		if (striker && striker->init())
		{
			striker->autorelease();
			return striker;
		}
		CC_SAFE_DELETE(striker);
		return NULL;
	};

	bool init();
	void tick(float dt);
	bool onContactBegin(PhysicsContact& contact);

private:
	bool mIsTouched = false;
};

#endif // __CARROM_STRIKER_H__
