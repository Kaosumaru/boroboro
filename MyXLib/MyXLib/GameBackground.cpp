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

class BerrySpawner : public Actor, public EffectWithCooldown
{
public:
	BerrySpawner()
	{

	}

	DWORD GetCooldownTime()
	{
		
		return 3000 * ((float)(rand()%100)/100.0f) * 2000;
	}

	float RandFloat()
	{
		return ((float)(rand()%1001) / 1000.0);
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



protected:

};


void InitBackground(MX::Scene *scene)
{
	_scene = scene;

	scene->AddActor(make_shared<Flower1>());
	scene->AddActor(make_shared<GameBackground>());
	scene->AddActor(make_shared<BerrySpawner>());
	scene->AddActor(make_shared<GameForeground>());
	
	//for (int i = 0; i < 10; i ++)
	//	scene->AddActor(make_shared<GameBackgroundGrass>());
		
}