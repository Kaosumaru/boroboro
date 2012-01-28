#include "stdafx.h"
#include "BloodAndGore.h"
#include "GameResources.h"
#include "../MXLib/MXParticles.h"

namespace MX
{


	class SikaczPrzedniCreator
	{
	public:
		SikaczPrzedniCreator() : 
			start_scaleX(1.0f, 1.0f),
			start_scaleY(1.0f, 1.0f),
			start_rotation(0.0f, 6.3f),
			end_rotation(0.0f, 6.3f),
			end_scaleX(1.0f, 1.0f),
			end_scaleY(1.0f, 1.0f),		
			speed(200.0f, 300.0f),
			life(1000,1000),
			direction(-1.0f, 1.0f),
			particle_z(0.0f,1.0f),
			start_color(0xDDFFFFFF, 0xBEFFFFF), end_color(0x00FFFFFF, 0x00FFFFFF)
		{
			animation = GraphicRes.blood;
		}

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

			float dir = original_direction + GetFloat(direction);

			particle->delta = dirVec(dir);



			particle->pos.x = start_x;
			particle->pos.y = start_y;

			particle->pos = particle->pos + particle->delta * 30.0f;

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
		{
			animation = GraphicRes.blood;
		}

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
			pos = target->pos;
			creator.original_direction = target->rotation;

			int particles = dispatcher.DispatchParticles();
			for (int i = 0; i < particles; i ++)
				creator.CreateParticle(this, scene, pos.x, pos.y);
			Actor::Do();
		}

		void SetTarget(ActorSprite *s)
		{
			target = s;
		}

		ParticleCreator creator;
		ParticleDispatcher dispatcher;
	protected:
		Scene *scene;
		ActorSprite *target;
	};

void AddFrontGore(MX::Scene *scene, MX::ActorSprite *body)
{
	typedef SikaczGenerator<SikaczPrzedniCreator, MX::SimpleParticleDispatcher<3,1> > SikaczPrzedni;
	
	auto sikacz = make_shared<SikaczPrzedni>(scene);
	sikacz->SetTarget(body);

	body->OnDeath.boost::signal<void (Actor&)>::connect(bind(&SikaczPrzedni::Die, sikacz));

	scene->AddActor(sikacz);	
}
void AddBackGore(MX::Scene *scene, MX::ActorSprite *body)
{
	typedef SikaczGenerator<SikaczCreator, MX::SimpleParticleDispatcher<3,1> > SikaczT;

	auto sikacz = make_shared<SikaczT>(scene);
	sikacz->SetTarget(body);
	sikacz->OnDo.connect(MX::q(wait(3000), die()));

	body->OnDeath.boost::signal<void (Actor&)>::connect(bind(&SikaczT::Die, sikacz));
	scene->AddActor(sikacz);
}


}


