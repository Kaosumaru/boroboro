#ifndef MXLIB_PARTICLES
#define MXLIB_PARTICLES

#include <windows.h>
#include <memory>
#include <vector>
#include "MXScene.h"
#include "MXWorld.h"
namespace MX
{


	class StandardParticle : public ActorSprite
	{
	public:
		StandardParticle(const shared_ptr<Animation> &anim = NULL) : animation(anim) {}
		

		float GetPercent()
		{
			float fP = (float)(World::GetCurrentTime() - start_stamp)/(float)(life_time);
			return fP > 1.0f ? 1.0f : fP;
		}

		void Do()
		{
			if (!alive)
				return;
			if (World::GetCurrentTime() > start_stamp + life_time)
			{
				alive = false;
				return;
			}
			float fP = GetPercent();

			x += dx * speed * World::GetElapsedFloat();
			y += dy * speed * World::GetElapsedFloat();
			
			//z -= World::GetElapsedFloat();

			float sX = LerpFloat(scaleX, dest_scaleX, fP);
			float sY = LerpFloat(scaleY, dest_scaleY, fP);

			float rot = LerpFloat(rotation, dest_rotation, fP);

			D3DXCOLOR c;
			D3DXColorLerp(&c, &color, &dest_color, fP);

			animation->Animate(*spriter, x, y, 0.0f, rot, sX, sY, c);

		}

		DWORD start_stamp;
		DWORD life_time;

		float speed;

		float dx;
		float dy;

		float dest_scaleX;
		float dest_scaleY;

		float dest_rotation;
		D3DXCOLOR dest_color;

		shared_ptr<Animation> animation;

	protected:
		float LerpFloat(float f1, float f2, float f)
		{
			return f1 + (f2 - f1) * f;
		}
	};

	template <DWORD freq=30, DWORD number=1>
	class SimpleParticleDispatcher
	{
	public:
		SimpleParticleDispatcher() : stamp(0) { }


		//returns number of particles to be dispatched in this iteration
		int DispatchParticles()
		{
			if (stamp == 0)
			{
				stamp = World::GetCurrentTime();
				return number;
			}
			if (stamp + freq < World::GetCurrentTime())
			{
				while (stamp + freq < World::GetCurrentTime()) 
					stamp += freq;
				return number;
			}
			return 0;
		}

	protected:
		DWORD stamp;
	};

	class SimpleParticleCreator
	{
	public:
		SimpleParticleCreator() : 
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

		void CreateParticle(Actor *parent, const shared_ptr<Scene> &scene, float start_x, float start_y)
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

			particle->dx = sin(dir);
			particle->dy = cos(dir);

			particle->x = start_x;
			particle->y = start_y;
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

	template <class ParticleCreator = SimpleParticleCreator, class ParticleDispatcher = SimpleParticleDispatcher>
	class ParticleGenerator : public Actor
	{
	public:
		ParticleGenerator(const shared_ptr<Scene> &_scene) : scene(_scene) {}

		void Do()
		{
			int particles = dispatcher.DispatchParticles();
			for (int i = 0; i < particles; i ++)
				creator.CreateParticle(this, scene, x, y);
			Actor::Do();
		}

		ParticleCreator creator;
		ParticleDispatcher dispatcher;
	protected:
		shared_ptr<Scene> scene;
	};

};

#endif