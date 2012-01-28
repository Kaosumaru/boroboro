#include "stdafx.h"
#include "Items.h"
#include "Game.h"
#include "GameResources.h"
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

GoodBootleItem::GoodBootleItem()
{
	item_image = GraphicRes.bottle;
}

void GoodBootleItem::Use(Scene *scene, Player *user)
{
	scene->AddActor(make_shared<BottleBoost>(user));
}



class ShieldPlayer : public Actor
{
public:
	ShieldPlayer(Player *u)
	{
		user = u;
		OnDo.connect(MX::q(wait(2000), die()));
		user->speed_multiplier = 0.0f;
		user->invisible = true;

		u->OnDo.connect(MX::q(lerp_color(0x80FFFFFF, 500), wait(1250), lerp_color(0xFFFFFFFF, 250)));

	for(auto next = dynamic_cast<PlayerSnake_Body*>(u->next_body_part);
		next != NULL;
		next = dynamic_cast<PlayerSnake_Body*>(next->GetButt()))
		next->OnDo.connect(MX::q(lerp_color(0x80FFFFFF, 500), wait(1250), lerp_color(0xFFFFFFFF, 250)));

	}


	void OnDie()
	{
		__super::OnDie();
		user->speed_multiplier = 1.0f;
		user->invisible = false;
	}

	void Do()
	{
		Actor::Do();
	}

protected:
	Player *user;
};

ShieldItem::ShieldItem()
{
	item_image = GraphicRes.shield;
}

void ShieldItem::Use(Scene *scene, Player *user)
{
	scene->AddActor(make_shared<ShieldPlayer>(user));
}

}