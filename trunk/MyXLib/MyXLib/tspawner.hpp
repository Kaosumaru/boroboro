#pragma once

#include "../MXLib/MXAnimation.h"
#include "../MXLib/MXScene.h"
#include "../MXLib/MXSpriter.h"
#include "Game.h"

extern MX::Scene *_scene;
namespace MX
{
template <class T>
class TSpawner : public Actor, public EffectWithCooldown
{
	Player *player;
	DWORD coolTime;
	int numTimes;
public:
	TSpawner(Player* p, DWORD ct, int num) : player(p), coolTime(ct), numTimes(num)
	{
	}
	DWORD GetCooldownTime()
	{
		return 200;
	}

	void DoEffect()
	{
		_scene->AddActor(make_shared<T>(_scene, player));
	}

	void Do()
	{
		if (CooldownElapsed())
		{
			DoEffect();
			if(--numTimes==0)
				Die();
		}
	}
};
}