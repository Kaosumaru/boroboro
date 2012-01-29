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

shared_ptr<MX::Animation> CreateAnimationFromFile(wchar_t* file, float cx, float cy)
{
	shared_ptr<MX::Image> cross(new MX::Image());
	bool b = cross->Load(*draw, file);

	shared_ptr<MX::Animation> anim(new MX::Animation(500));
	shared_ptr<MX::Frame> frame(new MX::Frame(cross));
	anim->AddFrame(MX::AnimationFrame(frame, 10000, cx, cy));
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

		if((this->head && this->scaleX >= 0.4) && !player->pentakill)
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
		{
			int oldLength = head->GetLength();
			head->RecalcLength();
			if(oldLength - head->GetLength() > 8)
			    SoundBank::no_ass.Play();
		}

		Die();
	}


	class PlayerTongue : public ActorSprite
	{
	public:
		PlayerTongue(const shared_ptr<ActorSprite> &pA) : ActorSprite(GraphicRes.snake_tongue) 
		{
			pActor = pA;
			z = pActor->z+0.1f;
			pos.x = 640.0f;
			pos.y = 400.0f;
		}

		void Do()
		{
			pos = pActor->pos;
			rotation = pActor->rotation;
			pos = pos + dirVec(rotation) * 64.0f;
			ActorSprite::Do();
		}
	protected:
		shared_ptr<ActorSprite> pActor;
	};

	
	Player::Player(const v2d& p, float d, bool alternative, int len) : Shield(10000)
	{
		score = 0;
		rotation_proportion = 75.0f;
		MovType = NormalControl;
		pentakill = false;

		item_pos.x = 370;
		item_pos.y = 770;

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

		for(int i =0; i<len; ++i)
			AddBodypart();


	}

	void Player::calculate_playerspeed()
	{
		Rotation_Speed = GetSpeed() / rotation_proportion;
	}


	void Player::KeyoardNavigate()
	{
		calculate_playerspeed();
		if (World::Key[KeyLeft] || MovType == ForceLeft)
			rotation -= Rotation_Speed * World::GetElapsedFloat();
		else if (World::Key[KeyRight] || MovType == ForceRight)
			rotation += Rotation_Speed * World::GetElapsedFloat();

		if (World::Key[KeyUse] && Item)
		{
			Item->Use(scene, this);
			Item = NULL;
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

		SoundBank::bump.Play();

		if(dadot < -0.7)
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


shared_ptr<MX::Player> player1, player2;


void InitializeGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
	SoundBank::AmbientSound.Stop();

	_scene->KillAll();
	draw = _draw;
	spriter = _spriter;
	scene = _scene;

	



	liner = shared_ptr<MX::Liner>(MX::Liner::Create(draw));
	liner->SetAntialias(true);
	

	InitBackground(_scene, _spriter);

	scene->AddActor(make_shared<MX::PlayerCrosshair>(*draw));

	player1 = make_shared<MX::Player>(v2d(150.0f, 400.0f), 1.57f, false);
	player2 = make_shared<MX::Player>(v2d(1000.0f, 400.0f), -1.57f, true);
	
	scene->AddActor(player1);


	player2->KeyLeft = 'A';
	player2->KeyRight = 'D';
	player2->KeyUse = 'W';
	player2->KeyShield = 'S';


	player2->item_pos.x = 900;


	scene->AddActor(player2);

	scene->AddActor(make_shared<MX::Flower1>());

	InitHighscore(draw, spriter, scene, player1, player2);




}


class WinInfo : public ActorSprite
{
public:
	WinInfo(int res) : ActorSprite(CreateAnimationFromFile(res == 0 ? L"images\\end_remis.png" : res == 1 ? L"images\\end_player1.png" : L"images\\end_player2.png")) 
	{
		color = 0x00FFFFFF;
		OnDo.connect(q(lerp_color(0xFFFFFFFF, 2000)));
		z = 0.5f;
		pos.x = 640.0f;
		pos.y = 400.0f;
	}

	void Do()
	{
		ActorSprite::Do();
		if (World::Key[VK_SPACE])
		{
			scene->KillAll();
			InitializeDemo(draw, ::spriter, scene);
		}
	}
};


class EndGamer : public ActorSprite
{
public:
	EndGamer(int res) : ActorSprite(CreateAnimationFromFile(L"images\\BlackFire.png"))
	{
		result = res;
		color = 0x00FFFFFF;
		OnDo.connect(q(lerp_color(0xFFFFFFFF, 4000)));
		OnDo.connect(q(warp_scale(60.0f, 60.0f, 4000), die()));
		pos.x = 640.0f;
		pos.y = 400.0f;
		z = 0.0f;
	}
	
	void OnDie(){
		ActorSprite::OnDie();
		scene->KillAll();
		player1 = NULL;
		player2 = NULL;

		auto info = make_shared<WinInfo>(result);

		//if (FirstPlayerWin)

		scene->AddActor(info);

	}

protected:
	int result;
};

void EndGame()
{
	int nResult = 0;
	if (player1->score > player2->score)
		nResult = 1;
	if (player1->score < player2->score)
		nResult = 2;


	scene->AddActor( make_shared<EndGamer>(nResult) );

}

class PentagramTitle : public ActorSprite
{
public:
	PentagramTitle(const std::shared_ptr<MX::Spriter> &s)
	{
		color = 0x00FFFFFF;
		spriter = s;

		pos.x = 640.0f;
		pos.y = 400.0f;
		z = 0.9f;

		scaleX = 4.0f;
		scaleY = 4.0f;

		animation = make_shared<SpecificAnimation>(GraphicRes.pentagram);
		animation->Start();

		OnDo.connect(q(wait(3500), warp_scale(0.5f, 0.5f, 500)));
		OnDo.connect(q(wait(3500), lerp_color(0xFFFFFFFF, 4000)));
		//OnDo.connect(q(wait(3500), lerp_color(0xFFFFFFFF, 4000)));

		/*
		auto loop = make_shared<LoopCommand> ();
		loop->AddCommand(make_shared<WarpScaleCommand>(0.23f, 0.23f, 250));
		loop->AddCommand(make_shared<WarpScaleCommand>(0.2f, 0.2f, 250));
		OnDo.connect(loop);*/
	}
	void Do(){
		rotation +=  0.005 * World::GetElapsedTime();
		ActorSprite::Do();
		GraphicRes.pentagram_static->Animate(*spriter, pos.x, pos.y, z , 0.0f, scaleX, scaleY, color);
	}

	std::shared_ptr<MX::Spriter> spriter;

};


class TitleEnd : public Actor
{
public:
	void Do()
	{
		Actor::Do();
		if (World::Key[VK_RETURN])
		{
			scene->KillAll();
			InitializeGame(draw, spriter, scene);
		}
	}
};


void InitializeDemo(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
//	SoundBank::AmbientSound.Play();
	draw = _draw;
	spriter = _spriter;
	scene = _scene;

	float cent = 640.0f, dist = 200.0f;
	auto player1 = make_shared<MX::Player>(v2d(cent-dist, 400.0f), 1.57f, false, 27);
	auto player2 = make_shared<MX::Player>(v2d(cent+dist, 400.0f), -1.57f, true, 27);


	player1->KeyLeft = 0;
	player1->KeyRight = 0;
	player1->KeyUse = 0;
	player1->KeyShield = 0;
	player1->rotation_proportion = 200.0f;


	player2->KeyLeft = 0;
	player2->KeyRight = 0;
	player2->KeyUse = 0;
	player2->KeyShield = 0;
	player2->rotation_proportion = 200.0f;


	player1->MovType = Player::ForceLeft;
	player2->MovType = Player::ForceLeft;


	//_scene->AddActor(make_shared<PlayerTongue>(player1));
	//_scene->AddActor(make_shared<PlayerTongue>(player2));

	_scene->AddActor(player1);
	_scene->AddActor(player2);
	_scene->AddActor(make_shared<PentagramTitle>(_spriter));

	

	auto press_enter = make_shared<ActorSprite>(CreateAnimationFromFile(L"images\\pressenter", 2, 450, 256, 64));
	press_enter->color = 0x00FFFFFF;
	press_enter->OnDo.connect(q(wait(4500), lerp_color(0xFFFFFFFF, 4000)));
	press_enter->pos.x = 640.0f;
	press_enter->pos.y = 700.0f;
	press_enter->z = 0.9f;
	_scene->AddActor(press_enter);


	auto BoroTitle = make_shared<ActorSprite>(CreateAnimationFromFile(L"images\\boroboro.png"));
	BoroTitle->color = 0x00FFFFFF;
	BoroTitle->OnDo.connect(q(wait(4500), lerp_color(0xFFFFFFFF, 4000)));
	BoroTitle->pos.x = 640.0f;
	BoroTitle->pos.y = 150.0f;
	BoroTitle->z = 0.0f;

	auto loop = make_shared<LoopCommand> ();
	loop->AddCommand(make_shared<WarpScaleCommand>(1.075f, 1.075f, 500));
	loop->AddCommand(make_shared<WarpScaleCommand>(1.0f, 1.0f, 500));
	BoroTitle->OnDo.connect(loop);


	_scene->AddActor(make_shared<ActorSprite>(CreateAnimationFromFile(L"images\\bkg.png", 0.0f, 0.0f)));
	_scene->AddActor(BoroTitle);

	_scene->AddActor(make_shared<TitleEnd>());

}

void initGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
	draw = _draw;
	spriter = _spriter;
	scene = _scene;
	
	GraphicRes.Init(*_draw);

}

