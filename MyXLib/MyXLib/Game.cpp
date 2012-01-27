#include "stdafx.h"
#include "game.h"
#include "GameMap.h"
#include "Sounds.h"
#include "../MXLib/MXParticles.h"
#include "../MXLib/MXAnimUtils.h"

shared_ptr<MX::Draw> draw;
shared_ptr<MX::Spriter> spriter;
MX::Scene *scene;
shared_ptr<MX::Liner> liner;

#define INVICIBLE_TIME 2000

namespace MX
{

shared_ptr<MX::Animation> CreateAnimationFromFile(wchar_t* file)
{
	shared_ptr<MX::Image> cross(new MX::Image());
	bool b = cross->Load(*draw, file);

	shared_ptr<MX::Animation> anim(new MX::Animation(500));
	shared_ptr<MX::Frame> frame(new MX::Frame(cross));
	anim->AddFrame(MX::AnimationFrame(frame, 10000, cross->Width()/2.0f, cross->Height()/2.0f));
	return anim;
}


shared_ptr<MX::Animation> CreateAnimationFromFile(wchar_t* file, int number, DWORD speed, float cx, float cy)
{
	wchar_t tfile[1024];
	shared_ptr<MX::Animation> anim(new MX::Animation(speed));

	for (int i = 0; i < number; i++)
	{
		shared_ptr<MX::Image> cross(new MX::Image());
		StringCbPrintf(tfile, 1024, L"%s%d.png", file, i+1);
		bool b = cross->Load(*draw, tfile);
		shared_ptr<MX::Frame> frame(new MX::Frame(cross));
		anim->AddFrame(MX::AnimationFrame(frame, speed, cx, cy));
	}

	return anim;
}


	void NormalizeDir(float &dx, float &dy)
	{
		float ilength = 1.f/sqrt(dx*dx + dy*dy);
		dx *= ilength;
		dy *= ilength;
	}


	class PlayerCrosshair : public ActorSprite
	{
	public:
		PlayerCrosshair(Draw &draw) : ActorSprite() 
		{
			z = 0.0f;
			animation = shared_ptr<SpecificAnimation>(new SpecificAnimation(CreateAnimationFromFile(L"images\\celownik.png")));
			animation->Start();
		}

		void Do()
		{
			x = (float)World::MouseX;
			y = (float)World::MouseY;
			__super::Do();
		}
	};


	
	class Player : public ActorSprite
	{
	public:
		Player()
		{
			Player_Direction = 0.0f;
			Rotation_Speed = 6.0f;
			Player_Speed = 400.0f;
			KeyLeft = VK_LEFT;
			KeyRight = VK_RIGHT;
		}
			

		void KeyoardNavigate()
		{
			if (World::Key[KeyLeft])
			{
				Player_Direction -= Rotation_Speed * World::GetElapsedFloat();
			}
			else if (World::Key[KeyRight])
			{
				Player_Direction += Rotation_Speed * World::GetElapsedFloat();
			}
		}

		void Move()
		{
			float dx, dy;
			dx = cos(Player_Direction);
			dy = sin(Player_Direction);

			x += dx * Player_Speed * World::GetElapsedFloat();
			y += dy * Player_Speed * World::GetElapsedFloat();
		}

		float Player_Direction; //direction in radians
		float Rotation_Speed;
		float Player_Speed;


		char KeyLeft;
		char KeyRight;
		char KeyUse;
	};



	class PlayerSnake_Body : public ActorSprite
	{
	public:
		PlayerSnake_Body(ActorSprite *previous)
		{
			scaleX = 0.5f;
			scaleY = 0.5f;

			z = previous->z+0.01f;

			animation = make_shared<SpecificAnimation>(CreateAnimationFromFile(L"images\\wonsz\\WonszBody.png"));
			animation->Start();

			previous_part = previous;
		}
			
		void Do()
		{
			float dx, dy;
			dx = cos(previous_part->rotation);
			dy = sin(previous_part->rotation);

			x = previous_part->x;
			y = previous_part->y;

			x -= dx*32;
			y -= dy*32;

			rotation = previous_part->rotation;
			__super::Do();
		}

	protected:

		ActorSprite *previous_part;

	};

	class PlayerSnake : public Player, public enable_shared_from_this<PlayerSnake>
	{
	public:
		PlayerSnake(Draw &draw) : Player() 
		{
			last_body_part = this;

			Player_Direction = 0.0f;

			scaleX = 0.5f;
			scaleY = 0.5f;

			x = 100.0f;
			y = 100.0f;
			z = 0.0f;
			animation = make_shared<SpecificAnimation>(CreateAnimationFromFile(L"images\\wonsz\\WonszGlowa.png"));
			animation->Start();

			AddBodypart();
			AddBodypart();
			AddBodypart();
		}

		void AddBodypart()
		{
			auto body_part = make_shared<PlayerSnake_Body>(last_body_part);

			last_body_part = &(*body_part);
			scene->AddActor(body_part);
		}

		void Do()
		{
			KeyoardNavigate();
			Move();
			Actor::rotation = Player_Direction;
			__super::Do();
		}
	protected:
		ActorSprite *last_body_part;
	};

}

void InitializeGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
	draw = _draw;
	spriter = _spriter;
	scene = _scene;


	liner = shared_ptr<MX::Liner>(MX::Liner::Create(draw));
	liner->SetAntialias(true);
	

	scene->AddActor(make_shared<MX::PlayerCrosshair>(*draw));

	scene->AddActor(make_shared<MX::PlayerSnake>(*draw));

}

void initGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
	draw = _draw;
	spriter = _spriter;
	scene = _scene;
	SoundBank::Initialize();

}

