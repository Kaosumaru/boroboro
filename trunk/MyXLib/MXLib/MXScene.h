#ifndef MXLIB_SCENE
#define MXLIB_SCENE

#include <windows.h>
#include <memory>
#include <vector>
#include <map>
#include "v2d.hpp"
#include "MXUtils.h"
#include "MXDraw.h"
#include "MXAnimation.h"
#include "MXSpriter.h"
namespace MX
{
	class StateMachine
	{
	public:
		StateMachine(int count = 1) { states.resize(count); }
		int GetCount() { states.size(); }
		void SetState(int newstate) { SetState(0, newstate); }
		void SetState(int which, int newstate) 
		{
			int o = states[which];
			states[which] = newstate;
			ChangeState(which, o, newstate);
		}

		int GetState(int which = 0) 
		{ 
			return states[which]; 
		}


		
	protected:
		virtual void ChangeState(int which, int oldstate, int newstate)
		{

		}

		vector<int> states;
	};

	class Actor;

	class Command
	{
	public:
		virtual ~Command(){}
		virtual bool operator () (Actor &actor) { return false; }
	};


	class ScriptSignal : public boost::signal<void (Actor&)>
	{
	public:
		ScriptSignal(){};

		void connect(const shared_ptr<Command>& com, int p = 0) { commands.insert(pair<int, shared_ptr<Command>>(p, com)); }
		void operator () (Actor &actor) 
		{
			auto it = commands.begin();
			while (it != commands.end())
			{
				if (!(*(it->second))(actor))
					it = commands.erase(it);
				else
					it ++;
			}
			boost::signal<void (Actor&)>::operator()(actor);
		}
	protected:
		multimap<int, shared_ptr<Command>> commands;
	};

	class Actor : public StateMachine
	{
	public:
		Actor()
		{
			z = -1.0f;
			//x = y = 0.0f;
			scaleX = scaleY = 1.0f;
			color = 0xFFFFFFFF;
			rotation = 0.0f;
			alive = true;
		}

		virtual ~Actor(){};

		virtual void Do();
		virtual void Die();

		bool alive;
	//aktor przechowuje pozycje
		//float x; 
		//float y;
		v2d pos;
		float z;
	//podstawowy kolor
		D3DXCOLOR color;
	//transformacje
		float scaleX;
		float scaleY;
		float rotation;
	//kszta³t (NULL), 


	//signals
		ScriptSignal OnDo;
		ScriptSignal OnDeath;
		boost::signal<void (Actor&,int,int,int)> OnChangeState;
	protected:
		friend class Scene;
		virtual void OnDie();

		void ChangeState(int which, int oldstate, int newstate)
		{
			OnChangeState(*this, which, oldstate, newstate);
		}
	};


	class ImageSprite : public Actor
	{
	public:
		ImageSprite(const shared_ptr<Image> &im = NULL);

		void Do();

		shared_ptr<Image> GetImage() const { return image; }
		void SetImage(shared_ptr<Image> val) { image = val; }

	protected:
		shared_ptr<Image> image;
		//float cx;
		//float cy;
		v2d cpos;
	};


	class ActorSprite : public Actor
	{
	public:
		ActorSprite(const shared_ptr<Animation> &anim = NULL);

		void Do();

		virtual void GetCurrentRect(float &_x, float &_y, float &_w, float &_h);

		AnimationFrame &GetCurrentFrame();
		bool Collide(ActorSprite &other);

		static Spriter *spriter;
	protected:
		virtual void DrawSprite();

		shared_ptr<SpecificAnimation> animation;
		D3DXMATRIX                   *custom_transform;
	};

	//czy sprite to rodzaj aktora? tak. upraszcza to caaaa³¹¹¹ logikê.
	//czy binding to rodzaj polecenia skryptu? Answer: tak! miejsce skryptu to sygna³, pod³¹czasz po prostu binding
	//czy particle to sprite'y? Answer: tak! maj¹ pozycje, transformacje, kolor... Mo¿emy sprawiæ, by by³y specjalnym rodzajem actora, by nie trzymaæ zbyt du¿o skryptów
	//czy aktorzy powinni mieæ dzieci? wtedy, kiedy aktor znikn¹³by, jego dzieci te¿.

	//podstawowe polecenia
	//wait, s, q, moveto(x,y,time,speed?), teleport(x,y), pulsate_color(color1, color2, time, speed), lerp_color(color1, color2, time), change_color(

	//przemysleæ kolizje

	class Scene
	{
	public:
		Scene();

		void Draw();
		void AddActor(const shared_ptr<Actor> &actor) { lActors.push_back(actor); }

		static shared_ptr<MX::Draw> draw;
	protected:

		list<shared_ptr<Actor>> lActors;
	}; 


};

#endif