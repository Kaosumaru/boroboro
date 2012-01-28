#include "stdafx.h"
#include "GameBackground.h"
#include "GameResources.h"
#include "../MXLib/MXScript.h"

using namespace MX;


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

void InitBackground(MX::Scene *scene)
{
	scene->AddActor(make_shared<Flower1>());
	scene->AddActor(make_shared<GameBackground>());

		
}