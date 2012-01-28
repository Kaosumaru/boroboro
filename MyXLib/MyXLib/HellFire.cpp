#include "stdafx.h"
#include "BloodAndGore.h"
#include "GameResources.h"
#include "../MXLib/MXParticles.h"

namespace MX
{


	class HellfireCreator
	{
	public:
		HellfireCreator() : 
			start_scaleX(0.25f, 0.25f),
			start_scaleY(0.25f, 0.25f),
			start_rotation(0.0f, 6.3f),
			end_rotation(0.0f, 6.3f),
			end_scaleX(0.25f, 0.25f),
			end_scaleY(0.25f, 0.25f),		
			speed(200.0f, 200.0f),
			life(250,450),
			direction(-0.2f, 0.2f),
			particle_z(0.5f,0.5f),
			start_color(0xFFFFEE00, 0xFFFEE00), end_color(0x00FF0000, 0x70FF0000)
		{
			animation = GraphicRes.fire;
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

			particle->speed = GetFloat(speed) + original_speed;

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
		float original_speed;
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
			creator.original_speed = target->GetSpeed();

			int particles = dispatcher.DispatchParticles();
			for (int i = 0; i < particles; i ++)
				creator.CreateParticle(this, scene, pos.x, pos.y);
			Actor::Do();
		}

		void SetTarget(Player *s)
		{
			target = s;
		}

		ParticleCreator creator;
		ParticleDispatcher dispatcher;
	protected:
		Scene *scene;
		Player *target;
	};

void AddHellFire(MX::Scene *scene, MX::Player *body)
{
	typedef SikaczGenerator<HellfireCreator, MX::SimpleParticleDispatcher<3,3> > SikaczPrzedni;
	
	auto sikacz = make_shared<SikaczPrzedni>(scene);
	sikacz->SetTarget(body);
	sikacz->OnDo.connect(MX::q(wait(3000), die()));

	body->OnDeath.boost::signal<void (Actor&)>::connect(bind(&SikaczPrzedni::Die, sikacz));

	scene->AddActor(sikacz);	
}



}


