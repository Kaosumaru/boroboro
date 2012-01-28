#include "stdafx.h"
#include "../MXLib/v2d.hpp"
#include "Game.h"
#include "GameMap.h"
#include "Sounds.h"
#include "Highscore.h"
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
	

	PlayerSnake_Body::PlayerSnake_Body(ActorSprite *bef, Player *player, bool bAlternative)
	{
		AlternativeLook = bAlternative;
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

		sharpenTail();

		dist = 64.0f * scaleX;

		pos = before->pos - dirVec(before->rotation)*dist;
		rotation = before->rotation; 
		toPos = before->pos;
		prevd = normalized(toPos-pos);
	}
	
	void PlayerSnake_Body::sharpenTail()
	{
		scaleX = scaleY = 0.38f;
		float lscale = scaleX;
		for(auto befo = dynamic_cast<PlayerSnake_Body*>(this->before);
			befo != NULL && this->before != head;
			befo = dynamic_cast<PlayerSnake_Body*>(befo->before))
		{
			lscale +=0.01f;
			befo->scaleX = befo->scaleY = lscale;				
			befo->dist = 48.0f * lscale;
			if(lscale > 0.58f)
			{
				befo->scaleX = befo->scaleY = 0.58f;
				//break;
				while(befo != NULL && this->before != head)
				{
					befo->scaleX = befo->scaleY = 0.58f;
					befo = dynamic_cast<PlayerSnake_Body*>(befo->before);
				}
				break;
			}
		}
	}


	void PlayerSnake_Body::newButt(ActorSprite *b)
	{
		butt = b;
	}

	float PlayerSnake_Body::GetSpeed()
	{
		return head ? head->GetSpeed() : 150.0f;
	}


	void PlayerSnake_Body::Do()
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

	void PlayerSnake_Body::onEat(Player* player)
	{
		//if(player->headonColCounter)
		//	return;
		//player->headonColCounter = 5;

		if(this->scaleX >= 0.4)
		{
			v2d normal = normalized(player->pos - pos);
			player->bounce(normal);
			return;
		}

		if (head && head->invisible)
			return;

		if (head == NULL && AlternativeLook == player->AlternativeLook)
			player->AddBodypart();


#if 0
		if (head != player)
			player->AddBodypart();
#endif

		SoundBank::snake_bite.Play();
		Urwij();
	}

	void PlayerSnake_Body::Urwij()
	{
		if (head)
		{
			head->last_body_part = before;
			PlayerSnake_Body* tail = dynamic_cast<PlayerSnake_Body*>(head->last_body_part);
			if(tail)
				tail->sharpenTail();
		}

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
	
		if (head)
			head->RecalcLength();

		Die();

	}





	Player::Player(const v2d& p, float d, bool alternative) : Shield(10000)
	{
		item_pos.x = 80;
		item_pos.y = 695;

		headonColCounter=0;
		length = 0;
		AlternativeLook = alternative;
		invisible = false;
		rotation = 0.0f;
		Rotation_Speed = 2.0f;
		speed = 150.0f;
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

		r = 24.0f;

		//x = 100.0f;
		//y = 100.0f;
		pos = p;
		z = 0.1f;
		animation = make_shared<SpecificAnimation>(AlternativeLook ? GraphicRes.snake_head2 : GraphicRes.snake_head);
		animation->Start();

		for(int i =0; i<30; ++i)
			AddBodypart();
	}

	void Player::calculate_playerspeed()
	{
		Rotation_Speed = GetSpeed() / 75.0f;
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

		}
	}

	void Player::DrawItems()
	{
		if (!Item || !Item->item_image)
			return;

		Item->item_image->Animate(*spriter, item_pos.x, item_pos.y);
	}

	void Player::Move()
	{
		v2d d = dirVec(rotation);
		pos = pos + d * GetSpeed() * World::GetElapsedFloat();
	}


	void Player::Do()
	{
		DrawItems();
		if(headonColCounter)
			--headonColCounter;
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
		float dadot = dot(dir, normal);
		if(dadot > 0)
			return;
		else if(dadot < -0.7)
		{
			this->rotation += sign(cross(dir, normal)) * 1.6; // must be * a little more than PI/2
			return;
		}
		v2d bou = dir - normal * (dadot * 2);
		float newrot = atan2(bou.y, bou.x);
		rotation = newrot;
	}

	void Player::onEat(Player* another)
	{
		if(headonColCounter || another->headonColCounter)
			return;
		headonColCounter = another->headonColCounter = 5;
		v2d normal = normalized(another->pos - pos);
		bounce(normal*-1);
		another->bounce(normal);
	}

	void Player::RecalcLength()
	{
		length = 0;
		for(auto next = dynamic_cast<PlayerSnake_Body*>(next_body_part);
			next != NULL;
			next = dynamic_cast<PlayerSnake_Body*>(next->butt))
			length ++;
	}


	
}

void Player::AddBodypart()
{
	length ++;

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

	SoundBank::Initialize();



	liner = shared_ptr<MX::Liner>(MX::Liner::Create(draw));
	liner->SetAntialias(true);
	

	InitBackground(_scene, _spriter);

	scene->AddActor(make_shared<MX::PlayerCrosshair>(*draw));

	auto player1 = make_shared<MX::Player>(v2d(150.0f, 400.0f), 1.57f, false);
	auto player2 = make_shared<MX::Player>(v2d(1000.0f, 400.0f), -1.57f, true);
	
	scene->AddActor(player1);

	player2->KeyLeft = 'A';
	player2->KeyRight = 'D';
	player2->KeyUse = 'W';
	player2->KeyShield = 'S';

	player2->item_pos.x = 1200;


	scene->AddActor(player2);

	scene->AddActor(make_shared<MX::Flower1>());

	InitHighscore(draw, spriter, scene, player1, player2);

}

void initGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
	draw = _draw;
	spriter = _spriter;
	scene = _scene;
	
	GraphicRes.Init(*_draw);

}

