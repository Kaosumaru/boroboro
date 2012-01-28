#include "stdafx.h"
#include "../MXLib/v2d.hpp"
#include "game.h"
#include "GameMap.h"
#include "Sounds.h"
#include "GameResources.h"
#include "Gamebackground.h"
#include "Collidable.h"
#include "HellFire.h"
#include "BloodAndgore.h"
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
		PlayerSnake_Body(ActorSprite *bef, Player *player, bool bAlternative)
		{
			speedMult = 1.0f;
			z = bef->z+0.00001f;

			animation = make_shared<SpecificAnimation>(bAlternative ? GraphicRes.snake_body2 : GraphicRes.snake_body);
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

			dist = 64.0f * scaleX;

			pos = before->pos - dirVec(before->rotation)*dist;
			rotation = before->rotation; 
			toPos = before->pos;
			prevd = normalized(toPos-pos);
		}
		
		void newButt(ActorSprite *b)
		{
			butt = b;
		}

		float GetSpeed()
		{
			return head ? head->GetSpeed() : 150.0f;
		}


		void Do()
		{
			if (before == NULL)
			{
				rotation += (rand()%100-50)*0.005f+(rand()%100-50)*0.005f+(rand()%100-50)*0.005f+(rand()%100-50)*0.005f;
				v2d d = dirVec(rotation);
				pos = pos + d * GetSpeed() * World::GetElapsedFloat();
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
			pos = pos + d * speedMult * GetSpeed() * World::GetElapsedFloat();

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
			if (head && head->invisible)
				return;


#if 0
			if (head != player)
				player->AddBodypart();
#endif
			Urwij();
		}

		void Urwij()
		{
#if 0
			auto part = make_shared<MX::ParticleGenerator<MX::SimpleParticleCreator, MX::SimpleParticleDispatcher<3,10>>>(scene);
			part->creator.SetAnimation(GraphicRes.blood);
			part->pos.x = pos.x;
			part->pos.y = pos.y;

			shared_ptr<MX::Command> com = MX::q(wait(250), die());
			part->OnDo.connect(com);
			scene->AddActor(part);
#endif

			if (head)
				head->last_body_part = before;

			PlayerSnake_Body * bef = dynamic_cast<PlayerSnake_Body*>(before);
			if (bef)
				bef->butt = NULL;	
			else if (head && head == before)
			{
				head->next_body_part = NULL;
				head = NULL;
				
			}

			if (before)
				AddBackGore(scene, before);




			if (butt)
			{
				PlayerSnake_Body * body = dynamic_cast<PlayerSnake_Body*>(butt);
				body->before = NULL;
				AddFrontGore(scene, butt);

				for(auto next = dynamic_cast<PlayerSnake_Body*>(butt);
					next != NULL;
					next = dynamic_cast<PlayerSnake_Body*>(next->butt))
					next->head = NULL;

				butt = NULL;
			}

		
			

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


	class ShieldPlayer : public Actor
	{
	public:
		ShieldPlayer(Player *u)
		{
			user = u;
			OnDo.connect(MX::q(wait(2000), die()));
			user->speed_multiplier = 0.0f;
			user->invisible = true;
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

	Player::Player(const v2d& p, float d, bool alternative) : Shield(1000)
	{
		AlternativeLook = alternative;
		invisible = false;
		rotation = 0.0f;
		Rotation_Speed = 2.0f;
		speed = 300.0f;
		speed_multiplier = 1.0f;
		KeyLeft = VK_LEFT;
		KeyRight = VK_RIGHT;
		KeyUse = VK_UP;
		KeyShield = VK_DOWN;
		next_body_part = NULL;
		last_body_part = this;

		rotation = d;

		scaleX = 0.65f;
		scaleY = 0.8f;

		//x = 100.0f;
		//y = 100.0f;
		pos = p;
		z = 0.1f;
		animation = make_shared<SpecificAnimation>(AlternativeLook ? GraphicRes.snake_head2 : GraphicRes.snake_head);
		animation->Start();

		for(int i =0; i<10; ++i)
			AddBodypart();
	}

	void Player::calculate_playerspeed()
	{
		Rotation_Speed = GetSpeed() / 50.0f;
	}


	void Player::KeyoardNavigate()
	{
		calculate_playerspeed();
		if (World::Key[KeyLeft])
		{
			rotation -= Rotation_Speed * World::GetElapsedFloat();
		}
		else if (World::Key[KeyRight])
		{
			rotation += Rotation_Speed * World::GetElapsedFloat();
		}
		else if (World::Key[KeyUse] && Item)
		{
			Item->Use(scene, this);
			Item = NULL;
		}
		else if (World::Key[KeyShield])
		{
			if (Shield.DoThis())
				scene->AddActor(make_shared<ShieldPlayer>(this));
				//AddHellFire(scene, this);
		}
	}

	void Player::Move()
	{
		v2d d = dirVec(rotation);
		pos = pos + d * GetSpeed() * World::GetElapsedFloat();
	}


	void Player::Do()
	{
		KeyoardNavigate();
		Move();
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

	void Player::bounce(const v2d& normal)
	{
	     v2d dir;
		 dir = dirVec(this->rotation);
		 v2d bou = dir + normal * (dot(dir, normal) * 2);
		 float newrot = atan2(bou.x, bou.y);
		 rotation = newrot;

		 /// @todo DO not always mirror 
	}

	void Player::onEat(Player* another)
	{
		v2d normal = normalized(another->pos - pos);
		bounce(normal);
		another->bounce(normal*-1);
	}
}

void Player::AddBodypart()
{
	if (last_body_part == NULL)
		last_body_part = this;
	auto body_part = make_shared<PlayerSnake_Body>(last_body_part, this, AlternativeLook);
	body_part->color = color;
	if (!next_body_part)
		next_body_part = body_part.get();
	PlayerSnake_Body* lastButt = dynamic_cast<PlayerSnake_Body*>(last_body_part);
	if(lastButt)
		lastButt->newButt(body_part.get());
	last_body_part = body_part.get();
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

	auto player1 = make_shared<MX::Player>(v2d(150.0f, 400.0f), 1.57f, false);
	auto player2 = make_shared<MX::Player>(v2d(1000.0f, 400.0f), -1.57f, true);
	
	scene->AddActor(player1);

	player2->KeyLeft = 'A';
	player2->KeyRight = 'D';
	player2->KeyUse = 'W';
	player2->KeyShield = 'S';


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

