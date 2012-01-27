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

#define BABA_HP 50.0f
#define BABA_DAMAGE 50.0f
#define BABA_COOLDOWN 1000
#define BABA_SPEED 90.0f
#define BABA_BULLET_SPEED 300.0f
#define BABA_STUN 750

#define SMALLGUY_HP 50.0f
#define SMALLGUY_DAMAGE 25.0f
#define SMALLGUY_COOLDOWN 100
#define SMALLGUY_SPEED 270.0f

#define SIMPLEENEMY_HP 100.0f
#define SIMPLEENEMY_SPEED 60.0f

#define DZIUBAK_HP 100.0f
#define DZIUBAK_SPEED 180.0f

#define TANK_HP 400.0f
#define TANK_SPEED 30.0f

#define CHAIN_LENGTH 300.0f
#define CHAIN_STRESS 100.0f
#define CHAIN_SLOW 0.3f


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
		float length = sqrt(dx*dx + dy*dy);
		dx /= length;
		dy /= length;
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
			Player_Speed = 5.0f;
		}
			

		void KeyoardNavigate()
		{
		}

		void Move()
		{
			float dx, dy;
			dx = sin(Player_Direction);
			dy = cos(Player_Direction);

			x += dx * Player_Speed;
			x += dy * Player_Speed;
		}

		float Player_Direction; //direction in radians
		float Rotation_Speed;
		float Player_Speed;


		char KeyLeft;
		char KeyRight;
		char KeyUse;
	};


	class PlayerSnake : public Player
	{
	public:
		PlayerSnake(Draw &draw) : Player() 
		{
			Player_Direction = 0.0f;

			scaleX = 0.5f;
			scaleY = 0.5f;

			x = 100.0f;
			y = 100.0f;
			z = 0.0f;
			animation = make_shared<SpecificAnimation>(CreateAnimationFromFile(L"images\\wonsz\\WonszGlowa.png"));
			animation->Start();
		}

		void Do()
		{
			Actor::rotation = Player_Direction;

			Move();

			
			__super::Do();
		}
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

