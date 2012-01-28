#include "stdafx.h"
#include "Items.h"
#include "Game.h"
#include "../MXLib/MXScript.h"

namespace MX
{

class BottleBoost : public Actor
{
public:
	BottleBoost(Player *u)
	{
		user = u;
		OnDo.connect(MX::q(wait(2000), die()));
		user->speed_multiplier = 2.0f;
	}


	void OnDie()
	{
		__super::OnDie();
		user->speed_multiplier = 1.0f;
	}

	void Do()
	{
		Actor::Do();
	}

protected:
	Player *user;
};

void GoodBootleItem::Use(Scene *scene, Player *user)
{
	scene->AddActor(make_shared<BottleBoost>(user));
}


}