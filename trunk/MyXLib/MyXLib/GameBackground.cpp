#include "stdafx.h"
#include "GameBackground.h"
#include "GameResources.h"
#include "Collidable.h"
#include "Game.h"
#include "../MXLib/MXScript.h"

using namespace MX;

MX::Scene *_scene;

class WarpRotationCommand : public WaitCommand
{
public:
	WarpRotationCommand(float rotation, unsigned mili) :  WaitCommand(mili), dRotation(rotation) { }

	bool operator () (Actor &actor) 
	{
		if (_stamp == 0)
			aRotation = actor.rotation;
		bool bRet = WaitCommand::operator()(actor);
		D3DXCOLOR t;
		//actor.x = aRotation + (dRotation - aRotation) * GetPercent();
		return bRet;
	}



	protected:
		float aRotation;
		float dRotation;

	};

shared_ptr<Command> warp_rotation(float nR, unsigned mili)
{
	return shared_ptr<Command>(new WarpRotationCommand(nR, mili));
}


class Flower1 : public ActorSprite
{
public:
	Flower1()
	{
		scaleX = 0.5f;
		scaleY = 0.5f;

		pos.x = 100.0f;
		pos.y = 100.0f;
		animation = make_shared<SpecificAnimation>(GraphicRes.flower1);
		animation->Start();
	}
};

class Pentagram : public ActorSprite
{
public:
	Pentagram()
	{
		scaleX = 0.2f;
		scaleY = 0.2f;

		pos.x = 200.0f;
		pos.y = 200.0f;
		animation = make_shared<SpecificAnimation>(GraphicRes.pentagram);
		animation->Start();
	}
	void Do(){
		rotation +=  0.005 * World::GetElapsedTime();
		ActorSprite::Do();
	}
};

class PentagramStatic : public ActorSprite
{
public:
	PentagramStatic()
	{
		scaleX = 0.2f;
		scaleY = 0.2f;

		pos.x = 200.0f;
		pos.y = 200.0f;
		animation = make_shared<SpecificAnimation>(GraphicRes.pentagram_static);
		animation->Start();
	}
};

class GameBackground : public ActorSprite
{
public:
	GameBackground()
	{
		z = 1.0f;
		pos.x = 0.0f;
		pos.y = 0.0f;
		animation = make_shared<SpecificAnimation>(GraphicRes.background);
		animation->Start();
	}
};

class GameForeground : public ActorSprite
{
public:
	GameForeground()
	{
		z = 0.0f;
		pos.x = 0.0f;
		pos.y = 0.0f;
		animation = make_shared<SpecificAnimation>(GraphicRes.foreground);
		animation->Start();
	}
};

class GameBackgroundGrass : public ActorSprite
{
public:
	GameBackgroundGrass()
	{
	//	scaleY = 0.25f;
		//scaleX = 0.25f;
		auto loop = make_shared<LoopCommand>();

		shear_factor = ((float)(rand()%100))/200.0f;

		custom_transform = &shear_transform;

		OnDo.connect(loop);

		pos.x = (float)(rand()%1280);
		pos.y = (float)(rand()%800);
		z = 1.0f-pos.y/800;

		animation = make_shared<SpecificAnimation>(GraphicRes.grass[rand()%3]);
		//animation = make_shared<SpecificAnimation>(GraphicRes.grass[3]);
		animation->Start();
	}


	void Do()
	{


	
	
		D3DXMatrixIdentity(&shear_transform);
		shear_transform._21 = cos((float)GetTickCount()/1000)*shear_factor;
		shear_transform._41 = - (pos.y+16.0f)*shear_transform._21 + shear_transform._21;

		ActorSprite::Do();
	}


	float                        shear_factor;
	D3DXMATRIX                   shear_transform;
	/*
	*/
};


class Berry : public Collidable
{
public:
	Berry()
	{
		z = 0.9f;

		pos.x = (float)(rand()%1280);
		pos.y = (float)(rand()%800);

		animation = make_shared<SpecificAnimation>(GraphicRes.berry);
		animation->Start();


		shared_ptr<MX::Command> com = MX::q(wait(5000), lerp_color(0x00FFFFFF, 1000), die());
		OnDo.connect(com);
	}

	void onEat(Player* player)
	{
		player->AddBodypart();
		player->AddBodypart();
		player->AddBodypart();
		Die();
	}
};

template<typename ItemType>
class BonusItem : public Collidable
{
public:
	BonusItem(const shared_ptr<Animation> &anim, int time = 5000, int vanish_time = 3000)
	{
		z = 0.9f;
		r = 16.0f;

		pos.x = (float)(rand()%1280);
		pos.y = (float)(rand()%800);

		animation = make_shared<SpecificAnimation>(anim);
		animation->Start();

		shared_ptr<MX::Command> com = MX::q(wait(time), lerp_color(0x00FFFFFF, vanish_time), die());
		OnDo.connect(com);
	}

	void onEat(Player* player)
	{
		player->Item = make_shared<ItemType>();
		Die();
	}
};


class BerrySpawner : public Actor, public EffectWithCooldown
{
public:
	DWORD GetCooldownTime()
	{
		return 3000 * ((float)(rand()%100)/100.0f) * 2000;
	}


	void DoEffect()
	{
		_scene->AddActor(make_shared<Berry>());
	}

	void Do()
	{
		if (CooldownElapsed())
			DoEffect();
	}
};


class BonusSpawner : public Actor, public EffectWithCooldown
{
public:
	DWORD GetCooldownTime()
	{
		return 20000 * ((float)(rand()%100)/100.0f) * 5000;
	}


	void DoEffect()
	{
		switch (rand() % 3)
		{
		case 0:
			_scene->AddActor(make_shared<BonusItem<GoodBootleItem>>(GraphicRes.bottle, 5000, 3000));
			break;
		case 1:
			_scene->AddActor(make_shared<BonusItem<PoopItem>>(GraphicRes.rotten_apple, 5000, 3000));
			break;
		case 2:
			_scene->AddActor(make_shared<BonusItem<ShieldItem>>(GraphicRes.shield, 5000, 3000));
			break;
		}

	}

	void Do()
	{
		if (CooldownElapsed())
			DoEffect();
	}
};


void InitBackground(MX::Scene *scene)
{
	_scene = scene;

	scene->AddActor(make_shared<Flower1>());
	scene->AddActor(make_shared<GameBackground>());
	scene->AddActor(make_shared<BerrySpawner>());
	scene->AddActor(make_shared<BonusSpawner>());
	scene->AddActor(make_shared<GameForeground>());
	scene->AddActor(make_shared<Pentagram>());
	scene->AddActor(make_shared<PentagramStatic>());
	
	//for (int i = 0; i < 10; i ++)
	//	scene->AddActor(make_shared<GameBackgroundGrass>());
		
}