#include "stdafx.h"
#include "../MXLib/v2d.hpp"
#include "game.h"
#include "GameMap.h"
#include "Sounds.h"
#include "GameResources.h"
#include "Gamebackground.h"
#include "Collidable.h"
#include "../MXLib/MXParticles.h"
#include "../MXLib/MXAnimUtils.h"
#include "../MXLib/MXParticles.h"

shared_ptr<MX::Draw> draw;
shared_ptr<MX::Spriter> spriter;
MX::Scene *scene;
shared_ptr<MX::Liner> liner;

#define INVICIBLE_TIME 2000

namespace MX
{


	class SikaczCreator
	{
	public:
		SikaczCreator() : 
			start_scaleX(1.0f, 1.0f),
			start_scaleY(1.0f, 1.0f),
			start_rotation(0.0f, 6.3f),
			end_rotation(0.0f, 6.3f),
			end_scaleX(1.0f, 1.0f),
			end_scaleY(1.0f, 1.0f),		
			speed(60.0f, 100.0f),
			life(1000,1000),
			direction(0.0f, 6.3f),
			particle_z(0.0f,1.0f),
			start_color(0xDDFFFFFF, 0xBEFFFFF), end_color(0x00FFFFFF, 0x00FFFFFF)
		{}

		void CreateParticle(Actor *parent, Scene *scene, float start_x, float start_y)
		{
			auto particle = make_shared<StandardParticle>(animation);
			particle->start_stamp = World::GetCurrentTime();
			
			particle->color = GetColor(start_color);
			particle->dest_color = GetColor(end_color);

			particle->life_time = GetDWORD(life);

			particle->scaleX = GetFloat(start_scaleX);
			particle->scaleY = GetFloat(start_scaleY);

			particle->dest_scaleX = GetFloat(end_scaleX);
			particle->dest_scaleY = GetFloat(end_scaleY);

			particle->rotation = GetFloat(start_rotation);
			particle->dest_rotation = GetFloat(end_rotation);

			particle->speed = GetFloat(speed);

			float dir = GetFloat(direction);

			//particle->delta.x = sin(dir);
			//particle->delta.y = cos(dir);
			particle->delta = dirVec(dir);

			particle->pos.x = start_x;
			particle->pos.y = start_y;
			particle->z = GetFloat(particle_z);

			scene->AddActor(particle);
		}

		void SetAnimation(const shared_ptr<Animation> &_animation) { animation = _animation; }



		pair<D3DXCOLOR, D3DXCOLOR> start_color, end_color;
		pair<DWORD, DWORD> life;

		pair<float, float> start_scaleX, start_scaleY;
		pair<float, float> end_scaleX, end_scaleY;

		pair<float, float> start_rotation, end_rotation;
		pair<float, float> direction, speed;
		pair<float, float> particle_z;

		float original_direction;
	protected:
		inline float GetRandomFloat() { return ((float) (rand() % 1001)/ 1000.0f); }

		float GetFloat(const pair<float, float> &pfloat)
		{
			return pfloat.first + (pfloat.second - pfloat.first) * GetRandomFloat();
		}

		DWORD GetColor(const pair<D3DXCOLOR, D3DXCOLOR> &color)
		{
			D3DXCOLOR c;
			D3DXColorLerp(&c, &color.first, &color.second, GetRandomFloat());
			return c;
		}

		DWORD GetDWORD(const pair<DWORD, DWORD> &pdword)
		{
			return pdword.first + (DWORD)((float)(pdword.second - pdword.first) * GetRandomFloat());
		}


		shared_ptr<Animation> animation;
	};

	template <class ParticleCreator = SikaczCreator, class ParticleDispatcher = SimpleParticleDispatcher>
	class SikaczGenerator : public Actor
	{
	public:
		SikaczGenerator(Scene *_scene) : scene(_scene) {}

		void Do()
		{
			int particles = dispatcher.DispatchParticles();
			for (int i = 0; i < particles; i ++)
				creator.CreateParticle(this, scene, pos.x, pos.y);
			Actor::Do();
		}

		ParticleCreator creator;
		ParticleDispatcher dispatcher;
	protected:
		Scene *scene;
	};


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
	

	class PlayerSnake_Body : public Collidable
	{
	public:
		PlayerSnake_Body(ActorSprite *bef, Player *player)
		{
			speedMult = 1.0f;
			z = bef->z+0.00001f;

			animation = make_shared<SpecificAnimation>(GraphicRes.snake_body);
			animation->Start();

			before = bef;
			head = player;
			butt = NULL;
			/*
			if(before == head)
				scaleX = scaleY = 0.9999999f * 0.54f;
			else
			{
				scaleX = before->scaleX * before->scaleX * float(1.0/0.54);
				scaleY = scaleX;
			}
			*/
			/*
			if(before == head)
				scaleX = scaleY = 0.65f;
			else
			{
				scaleX = 0.98f * before->scaleX;
				scaleY = 0.98f * before->scaleX;
			}
			*/

			scaleX = scaleY = 0.38f;
			for(auto befo = dynamic_cast<PlayerSnake_Body*>(before);
				befo != NULL && bef != head;
				befo = dynamic_cast<PlayerSnake_Body*>(befo->before))
			{
				befo->scaleX += 0.01f;
				befo->scaleY += 0.01f;
				//befo->scaleX = atan(before->scaleX*1.85f) * 0.634f;
				//befo->scaleY = atan(before->scaleY*1.85f) * 0.634f;
				befo->dist = 48.0f * befo->scaleX;
				if(befo->scaleX > 0.58f)
					break;
			}

			dist = 48.0f * scaleX;

			pos = before->pos - dirVec(before->rotation)*dist;
			rotation = before->rotation; 
			toPos = before->pos;
			prevd = normalized(toPos-pos);
		}
		
		void newButt(ActorSprite *b)
		{
			butt = b;
		}

		void Do()
		{
			if (before == NULL)
			{
				rotation += (rand()%100-50)*0.005f+(rand()%100-50)*0.005f+(rand()%100-50)*0.005f+(rand()%100-50)*0.005f;
				v2d d = dirVec(rotation);
				pos = pos + d * head->speed * World::GetElapsedFloat();
				__super::Do();
				return;
			}

			v2d d = toPos - pos;
			float ld = length(d);
			if(ld < 7.0f+dist*0.15f)
			{
				toPos = before->pos;
				d = toPos - pos;
			}
			v2d dd = before->pos - pos;
			float ldd = length(dd);
			speedMult = ldd/dist;
		
			speedMult = speedMult*speedMult;

			if(speedMult < 0.9f)
				speedMult = 0.9f;
			else if(speedMult > 1.1f)
				speedMult = 1.1f;

			d = normalized(d);
			dd = normalized(dd);
			d = d*2+ prevd*4.5f + dd;
			d = normalized(d);

			prevd = d;

			v2d prevPos = pos;
			pos = pos + d * speedMult * head->speed * World::GetElapsedFloat();

			if(length(pos - prevPos) > 4.0f)
			{
				d = dd;
			}

			if(butt)
			{
				v2d b = normalized(pos - butt->pos);
				dd = dd + b;
				dd = normalized(dd);
			}
			this->rotation = atan2(dd.y, dd.x); 

			__super::Do();
		}

		void onEat(Player* player)
		{
			Urwij();
		}

		void Urwij()
		{
			auto part = make_shared<MX::ParticleGenerator<MX::SimpleParticleCreator, MX::SimpleParticleDispatcher<3,10>>>(scene);
			part->creator.SetAnimation(GraphicRes.blood);
			part->pos.x = pos.x;
			part->pos.y = pos.y;

			shared_ptr<MX::Command> com = MX::q(wait(250), die());
			part->OnDo.connect(com);
			scene->AddActor(part);

			if (butt)
			{
				PlayerSnake_Body * body = dynamic_cast<PlayerSnake_Body*>(butt);
				body->before = NULL;
				butt = NULL;
			}

			/*
			typedef SikaczGenerator<SikaczCreator, SimpleParticleDispatcher<3,10> > SikaczT;
			auto sikacz = make_shared<SikaczT>(scene);

			OnDeath.connect(bind(&SikaczT::Die, sikacz));*/

			Die();

		}

		ActorSprite * GetButt() { return butt; }


		ActorSprite *before;  ///< before this segment
	protected:

		friend class Player;
		Player* head;
		ActorSprite *butt;    ///< after this segment
		float speedMult;
		float dist;
		v2d toPos;
		v2d prevd;
		//unsigned num;
	};

	Player::Player(const v2d& p, float d)
	{
		Player_Direction = 0.0f;
		Rotation_Speed = 2.0f;
		speed = 150.0f;
		KeyLeft = VK_LEFT;
		KeyRight = VK_RIGHT;
		next_body_part = NULL;
		last_body_part = this;

		Player_Direction = d; // 0.0f;
		this->rotation = d;

		scaleX = 0.65f;
		scaleY = 0.8f;

		//x = 100.0f;
		//y = 100.0f;
		pos = p;
		z = 0.1f;
		animation = make_shared<SpecificAnimation>(GraphicRes.snake_head);
		animation->Start();

		for(int i =0; i<10; ++i)
			AddBodypart();
	}

	void Player::calculate_playerspeed()
	{
		Rotation_Speed = speed / 50.0f;
	}


	void Player::KeyoardNavigate()
	{
		calculate_playerspeed();
		if (World::Key[KeyLeft])
		{
			Player_Direction -= Rotation_Speed * World::GetElapsedFloat();
		}
		else if (World::Key[KeyRight])
		{
			Player_Direction += Rotation_Speed * World::GetElapsedFloat();
		}
		else if (World::Key[VK_SPACE])
		{
			static EffectWithGivenCooldown boom(1000);

			if (boom.DoThis())
			{
				ActorSprite *test = next_body_part;
				

				for (int i = 0; i < 6; i ++)
				{
					PlayerSnake_Body * body = dynamic_cast<PlayerSnake_Body*>(test);
					test = body->GetButt();
					
				}

				PlayerSnake_Body * body = dynamic_cast<PlayerSnake_Body*>(test);
				body->Urwij();

			}

			/*
			if (boom.DoThis())
			{
			auto part = make_shared<MX::ParticleGenerator<MX::SimpleParticleCreator, MX::SimpleParticleDispatcher<3,10>>>(scene);
			part->creator.SetAnimation(GraphicRes.blood);
			part->pos.x = pos.x;
			part->pos.y = pos.y;

			shared_ptr<MX::Command> com = MX::q(wait(250), die());
			part->OnDo.connect(com);
			scene->AddActor(part);
			}*/

		}
	}

	void Player::Move()
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


	void Player::Do()
	{
		KeyoardNavigate();
		Move();
		Actor::rotation = Player_Direction;
		sw.allUCanEat(this);
		__super::Do();
	}

	static const unsigned NECK_MAX = 10;
	bool Player::isInNeck(Collidable* c)
	{
		unsigned n=0;
		for(auto next = dynamic_cast<PlayerSnake_Body*>(next_body_part);
			next != NULL;
			next = dynamic_cast<PlayerSnake_Body*>(next->butt))
		{
			if(next == c)
				return true;
			if(++n > NECK_MAX)
				break;
		}
		return false;
	}
}

void Player::AddBodypart()
{
	auto body_part = make_shared<PlayerSnake_Body>(last_body_part, this);
	if (!next_body_part)
		next_body_part = body_part.get();
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

	auto player1 = make_shared<MX::Player>(v2d(150.0f, 400.0f), 1.57f);
	auto player2 = make_shared<MX::Player>(v2d(1000.0f, 400.0f), -1.57f);
	
	scene->AddActor(player1);

	player2->KeyLeft = 'A';
	player2->KeyRight = 'D';

	scene->AddActor(player2);

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

