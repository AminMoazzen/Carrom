#ifndef __CARROM_STRIKER_H__
#define __CARROM_STRIKER_H__

#include "cocos2d.h"
#include "AudioEngine.h"

USING_NS_CC;

using namespace experimental;

class Striker : public Component
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

	bool setup(PhysicsMaterial& material, float damping);

private:
	bool mIsTouched = false;

	bool init();
	bool onContactBegin(PhysicsContact& contact);

	void PlayHitSound(float hitPower)
	{
		float volume = clampf(hitPower, 0, 1);
		AudioEngine::play2d("sounds/SFXDiskHit.mp3", false, volume);
	}

	void Pocket(cocos2d::Node* disk)
	{
		auto fadeOut = FadeOut::create(0.1f);
		auto playSound = CallFunc::create([=]() { AudioEngine::play2d("sounds/SFXDiskPocket.mp3"); });
		auto destroy = CallFunc::create([=]() { disk->removeFromParentAndCleanup(true); });
		auto seq = Sequence::create(playSound, fadeOut, destroy, nullptr);

		disk->runAction(seq);
	}
};

#endif // __CARROM_STRIKER_H__
