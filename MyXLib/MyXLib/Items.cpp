#include "stdafx.h"
#include "Items.h"
#include "Game.h"
#include "GameResources.h"
#include "Sounds.h"
#include "HellFire.h"
#include "../MXLib/MXScript.h"
#include <set>

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


class PoopPlayer : public Actor
{
public:
	PoopPlayer(Player *u)
	{
		user = u;
		OnDo.connect(MX::q(wait(3000), die()));
		user->speed_multiplier = 0.5f;
		

	}


	void OnDie()
	{
		__super::OnDie();
		user->speed_multiplier = 1.0f;
	}
protected:
	Player *user;
};


class PoopMine : public Collidable
{
public:
	PoopMine(Scene *s, Player *user)
	{
		scene = s;
		OnDo.connect(MX::q(wait(5000), lerp_color(0x00FFFFFF, 500), die()));

		r = 16.0f;
		pos = user->pos;

		auto next = dynamic_cast<PlayerSnake_Body*>(user->next_body_part);
		for(;next != NULL && next->GetButt() != NULL;next = dynamic_cast<PlayerSnake_Body*>(next->GetButt())){}

		if(!next)
		{
			// shit form head
			pos.x = user->pos.x;
			pos.y = user->pos.y;
		}
		pos.x = next->pos.x;
		pos.y = next->pos.y;

		animation = make_shared<SpecificAnimation>(GraphicRes.poop);
		animation->Start();
	}

	void onEat(Player* player)
	{
		if (hit_by_poop.insert(player).second)
			scene->AddActor(make_shared<PoopPlayer>(player));
	}

	std::set<Player*> hit_by_poop;
	Scene *scene;
};


PoopItem::PoopItem()
{
	item_image = GraphicRes.poop;
}

void PoopItem::Use(Scene *scene, Player *user)
{
	SoundBank::put_shit.Play();
	scene->AddActor(make_shared<PoopMine>(scene, user));
}


class PentagramPlayer : public Actor
{
public:
	PentagramPlayer(Player *u)
	{
		user = u;
		OnDo.connect(MX::q(wait(2000), die()));
		user->pentakill = true;
	}


	void OnDie()
	{
		__super::OnDie();
		//user->speed_multiplier = 1.0f;
		user->pentakill = false;
	}

protected:
	Player *user;
};

PentagramItem::PentagramItem()
{
	item_image = GraphicRes.pentagram_small;
}

void PentagramItem::Use(Scene *scene, Player *user)
{
	scene->AddActor(make_shared<PentagramPlayer>(user));
	SoundBank::flame.Play();
	AddHellFire(scene, user);
}


}