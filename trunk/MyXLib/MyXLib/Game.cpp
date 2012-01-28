#include "stdafx.h"
#include "../MXLib/v2d.hpp"
#include "game.h"
#include "GameMap.h"
#include "Sounds.h"
#include "GameResources.h"
#include "Gamebackground.h"
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
			pos.x = (float)World::MouseX;
			pos.y = (float)World::MouseY;
			__super::Do();
		}
	};

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
	
	class Player : public ActorSprite
	{
	public:
		Player()
		{
			Player_Direction = 0.0f;
			Rotation_Speed = 3.0f;
			speed = 100.0f;
			KeyLeft = VK_LEFT;
			KeyRight = VK_RIGHT;

			last_body_part = this;

			Player_Direction = 0.0f;

			scaleX = 0.8f;
			scaleY = 0.8f;

			//x = 100.0f;
			//y = 100.0f;
			pos = v2d(100.0f,100.0f);
			z = 0.0f;
			animation = make_shared<SpecificAnimation>(GraphicRes.snake_head);
			animation->Start();

			for(int i =0; i<100; ++i)
				AddBodypart();
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
			//float dx, dy;
			//dx = cos(Player_Direction);
			//dy = sin(Player_Direction);
			v2d d = dirVec(Player_Direction);

			//x += dx * speed * World::GetElapsedFloat();
			//y += dy * speed * World::GetElapsedFloat();
			pos = pos + d * speed * World::GetElapsedFloat();

			//std::wstringstream w;
			//w<<
			//OutputDebugString(
		}

		void AddBodypart();

		void Do()
		{
			KeyoardNavigate();
			Move();
			Actor::rotation = Player_Direction;
			__super::Do();
		}

		float Player_Direction; //direction in radians
		float Rotation_Speed;
		float speed;


		char KeyLeft;
		char KeyRight;
		char KeyUse;

		ActorSprite *last_body_part;

	};

	class PlayerSnake_Body : public ActorSprite
	{
	public:
		PlayerSnake_Body(ActorSprite *bef, Player *player)
		{
			scaleX = 0.5f;
			scaleY = 0.5f;
			speedMult = 1.0f;
			dist = 30.0f;
			z = bef->z+0.00001f;

			animation = make_shared<SpecificAnimation>(GraphicRes.snake_body);
			animation->Start();

			before = bef;
			toPos = before->pos;
			head = player;
			butt = NULL;
			pos = before->pos - dirVec(before->rotation)*dist; 
		}
		
		void newButt(ActorSprite *b)
		{
			butt = b;
		}

		void Do()
		{
			v2d d = toPos - pos;
			float ld = length(d);
			if(ld < 5)
			{
				toPos = before->pos;
				d = toPos - pos;
			}
			v2d dd = before->pos - pos;
			float ldd = length(dd);
			speedMult = ldd/dist;
			/*if(speedMult < 0.9f)
				speedMult = 0.9f;
			else if(speedMult > 1.1f)
				speedMult = 1.1f;
				*/
			speedMult = speedMult*speedMult;
			
			d = normalized(d);
			pos = pos + d * speedMult * head->speed * World::GetElapsedFloat();

			dd = normalized(dd);
			if(butt)
			{
				v2d b = normalized(pos - butt->pos);
				dd = dd + b;
				dd = normalized(dd);
			}
			this->rotation = atan2(dd.y, dd.x); 

			__super::Do();
		}

	protected:

		ActorSprite *before;  ///< before this segment
		Player* head;
		ActorSprite *butt;    ///< after this segment
		float speedMult;
		float dist;
		v2d toPos;
	};
}

void Player::AddBodypart()
{
	auto body_part = make_shared<PlayerSnake_Body>(last_body_part, this);
	PlayerSnake_Body* lastButt = dynamic_cast<PlayerSnake_Body*>(last_body_part);
	if(lastButt)
		lastButt->newButt(&(*body_part));
	last_body_part = &(*body_part);
	scene->AddActor(body_part);
}



void InitializeGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
	draw = _draw;
	spriter = _spriter;
	scene = _scene;


	liner = shared_ptr<MX::Liner>(MX::Liner::Create(draw));
	liner->SetAntialias(true);
	

	InitBackground(_scene);

	scene->AddActor(make_shared<MX::PlayerCrosshair>(*draw));

	scene->AddActor(make_shared<MX::Player>());

	scene->AddActor(make_shared<MX::Flower1>());

}

void initGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
	draw = _draw;
	spriter = _spriter;
	scene = _scene;
	SoundBank::Initialize();

	GraphicRes.Init(*_draw);

}

