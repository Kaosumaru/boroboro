#include "glm/vec2.hpp"
#include "Player.h"
#include "script/PropertyLoaders.h"

#define INVICIBLE_TIME 2000

namespace Boro
{
	PlayerSnake_Body::PlayerSnake_Body(SpriteActor *bef, Player *player, const std::shared_ptr<MX::Graphic::Image> &image)
		: MX::ImageSpriteAdapter<Collidable>(image)
	{
		before = bef;
		owner = head = player;
		butt = nullptr;

		sharpenTail();

		dist = 64.0f * geometry.scale.x;

		geometry.position = before->geometry.position - MX::CreateVectorFromAngle(before->geometry.angle)*dist;
		geometry.angle = before->geometry.angle; 
		toPos = before->geometry.position;
		prevd = glm::normalize(toPos-geometry.position);

	}
	
	void PlayerSnake_Body::sharpenTail()
	{
		geometry.scale.x = geometry.scale.y = 0.38f;
		float lscale = geometry.scale.x;
		for(auto befo = dynamic_cast<PlayerSnake_Body*>(this->before);
			befo != NULL && this->before != head;
			befo = dynamic_cast<PlayerSnake_Body*>(befo->before))
		{
			lscale +=0.01f;
			befo->geometry.scale.x = befo->geometry.scale.y = lscale;				
			befo->dist = 48.0f * lscale;
			if(lscale > 0.58f)
			{
				befo->geometry.scale.x = befo->geometry.scale.y = 0.58f;
				//break;
				while(befo != NULL && this->before != head)
				{
					befo->geometry.scale.y = befo->geometry.scale.y = 0.58f;
					befo = dynamic_cast<PlayerSnake_Body*>(befo->before);
				}
				break;
			}
		}
	}


	void PlayerSnake_Body::newButt(SpriteActor *b)
	{
		butt = b;
	}

	float PlayerSnake_Body::GetSpeed()
	{
		return head ? head->GetSpeed() : 150.0f;
	}

	void PlayerSnake_Body::Draw(float x, float y)
	{
		MX::ImageSpriteAdapter<Collidable>::Draw(x, y);
	}

	void PlayerSnake_Body::Run()
	{
		if (before == NULL)
		{
			geometry.angle += (rand()%100-50)*0.005f+(rand()%100-50)*0.005f+(rand()%100-50)*0.005f+(rand()%100-50)*0.005f;
			glm::vec2 d = MX::CreateVectorFromAngle(geometry.angle);
			geometry.position = geometry.position + d * GetSpeed() * (float)MX::Time::Timer::current().elapsed_seconds();
			Collidable::Run();
			return;
		}

		auto &pos = geometry.position;
		glm::vec2 d = toPos - pos;
		float ld = length(d);
		if(ld < 7.0f+dist*0.15f)
		{
			toPos = before->geometry.position;
			d = toPos - pos;
		}
		glm::vec2 dd = before->geometry.position - pos;
		float ldd = length(dd);
		speedMult = ldd/dist;
	
		speedMult = speedMult*speedMult;

		if(speedMult < 0.9f)
			speedMult = 0.9f;
		else if(speedMult > 1.1f)
			speedMult = 1.1f;

		d = glm::normalize(d);
		dd = glm::normalize(dd);
		d = d*2.0f+ prevd*4.5f + dd;
		d = glm::normalize(d);

		prevd = d;

		glm::vec2 prevPos = pos;
		pos = pos + d * speedMult * GetSpeed() * (float)MX::Time::Timer::current().elapsed_seconds();

		if(length(pos - prevPos) > 4.0f)
		{
			d = dd;
		}

		if(butt)
		{
			glm::vec2 b = glm::normalize(pos - butt->geometry.position);
			dd = dd + b;
			dd = glm::normalize(dd);
		}
		this->geometry.angle = MX::angle(dd); //TES 

		Collidable::Run();

	}

	void PlayerSnake_Body::onEat(Player* player)
	{
		if((this->head && this->geometry.scale.x >= 0.4) && !player->pentakill)
		{
			glm::vec2 normal = glm::normalize(player->geometry.position - geometry.position);
			player->bounce(normal);
			return;
		}

		if (head && head->invisible)
			return;

		//regain body if eaten by you
		if (head == NULL && owner == player)
			player->AddBodypart();


#if 0
		if (head != player)
			player->AddBodypart();
#endif

#ifdef WIP
		SoundBank::snake_bite.Play();
#endif
		Urwij();
	}

	void PlayerSnake_Body::Urwij()
	{
#ifdef WIP
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
#endif
	}

	
	Player::Player(MX::LScriptObject& script) : MX::AnimatedSpriteAdapter<Collidable>(script)
	{
		using namespace MX;
		{
			Graphic::SingleAnimation::pointer single_animation;
			script.load_property_child(single_animation, "Animation");


			Graphic::Animation::pointer animation;
			if (single_animation)
				animation = Graphic::Animation::Create(single_animation);
			SetAnimation(animation);
		}

		script.load_property(_controller, "Controller");

		script.load_property(item_pos, "Item.Pos");

		script.load_property(_body, "Body");

		int len = 0;
		script.load_property(len, "Length");

		for(int i =0; i<len; ++i)
			AddBodypart();
	}

	void Player::calculate_playerspeed()
	{
		Rotation_Speed = GetSpeed() / rotation_proportion;
	}


	void Player::Navigate()
	{
		calculate_playerspeed();

		if (!_controller)
			return;

		geometry.angle += _controller->direction() * (float)MX::Time::Timer::current().elapsed_seconds();

		if (_controller->useItem())
		{
#ifdef WIP
			Item->Use(scene, this);
			Item = nullptr;
#endif
		}
	}

	void Player::DrawItems()
	{
#ifdef WIP
		if (!Item || !Item->item_image)
			return;

		Item->item_image->Animate(*spriter, item_pos.x, item_pos.y);
#endif
	}

	void Player::Move()
	{
		glm::vec2 d = MX::CreateVectorFromAngle(geometry.angle);
		geometry.position += d * GetSpeed() * (float)MX::Time::Timer::current().elapsed_seconds();
	}


	void Player::Run()
	{
		SphereWorld& sw = SphereWorld::getInst();

#ifdef WIP
		DrawItems();
#endif
		if(headonColCounter)
			--headonColCounter;

		Navigate();
		Move();
		sw.allUCanEat(this);

		MX::AnimatedSpriteAdapter<Collidable>::Run();
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

	void Player::bounce(const glm::vec2& normal)
	{
		glm::vec2 dir;
		dir = MX::CreateVectorFromAngle(geometry.angle);
		float dadot = dot(dir, normal);
		if(dadot > 0)
			return;

#ifdef WIP
		SoundBank::bump.Play();
#endif

		auto cross = [](const glm::vec2& v1, const glm::vec2& v2)
		{
			return (v1.x*v2.y) - (v1.y*v2.x);
		};

		if(dadot < -0.7)
		{
			geometry.angle += glm::sign(cross(dir, normal)) * 1.6; // must be * a little more than PI/2
			return;
		}
		glm::vec2 bou = dir - normal * (dadot * 2);
		float newrot = atan2(bou.y, bou.x);
		geometry.angle = newrot;
	}

	void Player::onEat(Player* another)
	{
		if(headonColCounter || another->headonColCounter)
			return;
		headonColCounter = another->headonColCounter = 5;
		glm::vec2 normal = glm::normalize(another->geometry.position - geometry.position);
		bounce(normal*-1.0f);
		another->bounce(normal);
	}

	void Player::RecalcLength()
	{
		length = 0;
		for(auto next = dynamic_cast<PlayerSnake_Body*>(next_body_part);
			next != nullptr;
			next = dynamic_cast<PlayerSnake_Body*>(next->butt))
			length ++;
	}


	void Player::AddBodypart()
	{
		length ++;

		if (last_body_part == nullptr)
			last_body_part = this;
		auto body_part = make_shared<PlayerSnake_Body>(last_body_part, this, _body);
		body_part->geometry.color = geometry.color;
		if (!next_body_part)
			next_body_part = body_part.get();
		PlayerSnake_Body* lastButt = dynamic_cast<PlayerSnake_Body*>(last_body_part);
		if(lastButt)
			lastButt->newButt(body_part.get());
		last_body_part = body_part.get();

		world().AddActorAtFront(body_part);
	}	
}



#ifdef WIP
shared_ptr<MX::Player> player1, player2;


void InitializeGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
	MX::GameMusic.Play();
	SoundBank::AmbientSound.Stop();

	_scene->KillAll();
	draw = _draw;
	spriter = _spriter;
	scene = _scene;

	



	liner = shared_ptr<MX::Liner>(MX::Liner::Create(draw));
	liner->SetAntialias(true);
	

	InitBackground(_scene, _spriter);

	player1 = make_shared<MX::Player>(glm::vec2(150.0f, 400.0f), 1.57f, false);
	player2 = make_shared<MX::Player>(glm::vec2(1000.0f, 400.0f), -1.57f, true);
	
	scene->AddActor(player1);


	player2->KeyLeft = 'A';
	player2->KeyRight = 'D';
	player2->KeyUse = 'W';
	player2->KeyShield = 'S';


	player2->item_pos.x = 900;


	scene->AddActor(player2);


	InitHighscore(draw, spriter, scene, player1, player2);




}


class WinInfo : public SpriteActor
{
public:
	WinInfo(int res) : SpriteActor(CreateAnimationFromFile(res == 0 ? L"images\\end_remis.png" : res == 1 ? L"images\\end_player1.png" : L"images\\end_player2.png")) 
	{
		color = 0x00FFFFFF;
		OnDo.connect(q(lerp_color(0xFFFFFFFF, 2000)));
		z = 0.5f;
		pos.x = 640.0f;
		pos.y = 400.0f;
	}

	void Do()
	{
		SpriteActor::Do();
		if (World::Key[VK_SPACE])
		{
			scene->KillAll();
			InitializeDemo(draw, ::spriter, scene);
		}
	}
};


class EndGamer : public SpriteActor
{
public:
	EndGamer(int res) : SpriteActor(CreateAnimationFromFile(L"images\\BlackFire.png"))
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
		SpriteActor::OnDie();
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
	MX::GameMusic.Stop();
	int nResult = 0;
	if (player1->score > player2->score)
		nResult = 1;
	if (player1->score < player2->score)
		nResult = 2;


	scene->AddActor( make_shared<EndGamer>(nResult) );

}

class PentagramTitle : public SpriteActor
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
		SpriteActor::Do();
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
	SoundBank::AmbientSound.Play();
	draw = _draw;
	spriter = _spriter;
	scene = _scene;

	float cent = 640.0f, dist = 200.0f;
	auto player1 = make_shared<MX::Player>(glm::vec2(cent-dist, 400.0f), 1.57f, false, 27);
	auto player2 = make_shared<MX::Player>(glm::vec2(cent+dist, 400.0f), -1.57f, true, 27);


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

	

	auto press_enter = make_shared<SpriteActor>(CreateAnimationFromFile(L"images\\pressenter", 2, 450, 256, 64));
	press_enter->color = 0x00FFFFFF;
	press_enter->OnDo.connect(q(wait(4500), lerp_color(0xFFFFFFFF, 4000)));
	press_enter->pos.x = 640.0f;
	press_enter->pos.y = 700.0f;
	press_enter->z = 0.9f;
	_scene->AddActor(press_enter);


	auto BoroTitle = make_shared<SpriteActor>(CreateAnimationFromFile(L"images\\boroboro.png"));
	BoroTitle->color = 0x00FFFFFF;
	BoroTitle->OnDo.connect(q(wait(4500), lerp_color(0xFFFFFFFF, 4000)));
	BoroTitle->pos.x = 640.0f;
	BoroTitle->pos.y = 150.0f;
	BoroTitle->z = 0.0f;

	auto loop = make_shared<LoopCommand> ();
	loop->AddCommand(make_shared<WarpScaleCommand>(1.075f, 1.075f, 500));
	loop->AddCommand(make_shared<WarpScaleCommand>(1.0f, 1.0f, 500));
	BoroTitle->OnDo.connect(loop);


	_scene->AddActor(make_shared<SpriteActor>(CreateAnimationFromFile(L"images\\bkg.png", 0.0f, 0.0f)));
	_scene->AddActor(BoroTitle);

	_scene->AddActor(make_shared<TitleEnd>());

}

void initGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene)
{
	draw = _draw;
	spriter = _spriter;
	scene = _scene;
	
	GraphicRes.Init(*_draw);

	MX::GameMusic.LoadStream(L"music\\game.mp3" );
}

#endif