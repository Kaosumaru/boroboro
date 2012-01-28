#ifndef MXLIB_SCRIPT
#define MXLIB_SCRIPT

#include <windows.h>
#include <memory>
#include <vector>
#include "MXUtils.h"
#include "MXDraw.h"
#include "MXAnimation.h"
#include "MXScene.h"
#include "MXWorld.h"
namespace MX
{


	template <typename T>
	class LambdaWrapper : public Command
	{
	public:
		LambdaWrapper(T _t) : t(_t) {}
		bool operator () (Actor &actor) { return t(actor); }
	protected:
		T t;
	};

	
	/*
	** Waits given time, then dies
	*/
	class WaitCommand : public Command
	{
	public:
		WaitCommand(unsigned mili) : _mili(mili) { _stamp = 0; }
		
		bool operator () (Actor &actor) 
		{
			if (_stamp == 0)
				_stamp = World::GetCurrentTime();
			if (World::GetCurrentTime() > _stamp + _mili)
				return false; 
			return true;
		}

		float GetPercent()
		{
			float fP = (float)(World::GetCurrentTime() - _stamp)/(float)(_mili);
			return fP > 1.0f ? 1.0f : fP;
		}

		void Clear() { _stamp = 0; }

	protected:
		unsigned _mili;
		unsigned _stamp;
	};

	shared_ptr<Command> wait(unsigned time);



	class MoveToCommand : public Command
	{
	public:
		MoveToCommand(float x, float y, float speed) : _x(x), _y(y), _speed(speed) {}

		bool operator () (Actor &actor) 
		{
			float dx = _x - actor.pos.x;
			float dy = _y - actor.pos.y;
			float dist = sqrt(dx*dx + dy*dy);
			dx/=dist;
			dy/=dist;
			dx*=_speed*World::GetElapsedFloat();
			dy*=_speed*World::GetElapsedFloat();

			if ((actor.pos.x  < _x) != (actor.pos.x + dx <= _x))
				actor.pos.x = _x;
			else
				actor.pos.x += dx;
			if ((actor.pos.y  < _y) != (actor.pos.y + dy <= _y))
				actor.pos.y = _y;
			else
				actor.pos.y += dy;
			return actor.pos.x != _x || actor.pos.y != _y;
		}

	protected:
		float _speed;
		float _x, _y;
	};

	shared_ptr<Command> move_to(float x, float y, float speed);


	class TeleportToCommand : public Command
	{
	public:
		TeleportToCommand(float x, float y) : _x(x), _y(y){}

		bool operator () (Actor &actor) 
		{
			actor.pos.x = _x;
			actor.pos.y = _y;
			return false;
		}

	protected:
		float _x, _y;
	};

	shared_ptr<TeleportToCommand> teleport_to(float x, float y);

	class ChangeColorCommand : public Command
	{
	public:
		ChangeColorCommand(D3DXCOLOR c) : color(c){}

		bool operator () (Actor &actor) 
		{
			actor.color = color;
			return false;
		}

	protected:
		D3DXCOLOR color;
	};

	shared_ptr<Command> change_color(D3DXCOLOR c);



	class DieCommand : public Command
	{
	public:
		DieCommand() {}

		bool operator () (Actor &actor) 
		{
			actor.alive = false;
			return false;
		}

	};

	shared_ptr<Command> die();

	class LerpColorCommand : public WaitCommand
	{
	public:
		LerpColorCommand(D3DXCOLOR color, unsigned mili) :  WaitCommand(mili), _color(color) { }
		
		bool operator () (Actor &actor) 
		{
			if (_stamp == 0)
				_basecolor = actor.color;
			bool bRet = WaitCommand::operator()(actor);
			D3DXCOLOR t;
			actor.color = *D3DXColorLerp(&t, &actor.color, &_color, GetPercent());
			return bRet;
		}



	protected:
		D3DXCOLOR _color, _basecolor;
	};

	shared_ptr<Command> lerp_color(D3DXCOLOR c, unsigned time);


	class WarpScaleCommand : public WaitCommand
	{
	public:
		WarpScaleCommand(float scaleX, float scaleY, unsigned mili) :  WaitCommand(mili), dScaleX(scaleX),  dScaleY(scaleY) { }

		bool operator () (Actor &actor) 
		{
			if (_stamp == 0)
			{
				aScaleX = actor.scaleX;
				aScaleY = actor.scaleY;
			}
			bool bRet = WaitCommand::operator()(actor);
			D3DXCOLOR t;
			actor.scaleX = aScaleX + (dScaleX - aScaleX) * GetPercent();
			actor.scaleY = aScaleY + (dScaleY - aScaleY) * GetPercent();
			return bRet;
		}



	protected:
		float aScaleX, aScaleY;
		float dScaleX, dScaleY;

	};

	shared_ptr<Command> warp_scale(float scaleX, float scaleY, unsigned mili);



	class QueueCommand : public Command
	{
	public:
		void AddCommand(const shared_ptr<Command>& command)
		{
			commands.push_back(command);
		}

		bool operator () (Actor &actor)
		{ 
			while(!commands.empty())
			{
				if ((*commands.front())(actor))
					return true;
				commands.pop_front();
			}
			return false;
		}

	protected:
		list<shared_ptr<Command>> commands;
	};


	class LoopCommand : public Command
	{
	public:
		LoopCommand() { current = commands.end(); }

		void AddCommand(const shared_ptr<Command>& command)
		{
			commands.push_back(command);
		}

		bool operator () (Actor &actor)
		{ 
			if (current == commands.end())
			{
				for_each(commands.begin(), commands.end(), [](const shared_ptr<Command> &ptr){ptr->Clear();});
				current = commands.begin();
			}

			if (current == commands.end())
				return true;

			while (!(**current)(actor))
			{
				current ++;
				if (current == commands.end())
					return true;
			}

			return true;
		}

	protected:
		list<shared_ptr<Command>> commands;
		list<shared_ptr<Command>>::iterator current;
	};
	
	class SimultaneusCommand : public Command
	{
	public:
		void AddCommand(const shared_ptr<Command>& command)
		{
			commands.push_back(command);
		}

		bool operator () (Actor &actor)
		{ 
			auto it = commands.begin();
			while (it != commands.end())
			{
				if (!(*(*it))(actor))
					it = commands.erase(it);
				else
					++ it;
			}
			return !commands.empty();
		}

	protected:
		list<shared_ptr<Command>> commands;
	};

	shared_ptr<Command> To_Command(const shared_ptr<Command> &t1);

	template <typename T1> shared_ptr<Command> To_Command(const T1 &t1)
	{
		return shared_ptr<Command>(new LambdaWrapper<T1>(t1));
	}
	
	template <typename T1> shared_ptr<QueueCommand> q(const T1 &t1)
	{
		shared_ptr<QueueCommand> queue(new QueueCommand);
		queue->AddCommand(To_Command(t1));
		return queue;
	}

	template <typename T1, typename T2> shared_ptr<QueueCommand> q(const T1 &t1, const T2 &t2)
	{
		shared_ptr<QueueCommand> queue(new QueueCommand);
		queue->AddCommand(To_Command(t1));
		queue->AddCommand(To_Command(t2));
		return queue;
	}

	template <typename T1, typename T2, typename T3> shared_ptr<QueueCommand> q(const T1 &t1, const T2 &t2, const T3 &t3)
	{
		shared_ptr<QueueCommand> queue(new QueueCommand);
		queue->AddCommand(To_Command(t1));
		queue->AddCommand(To_Command(t2));
		queue->AddCommand(To_Command(t3));
		return queue;
	}

	template <typename T1, typename T2, typename T3, typename T4> shared_ptr<QueueCommand> q(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4)
	{
		shared_ptr<QueueCommand> queue(new QueueCommand);
		queue->AddCommand(To_Command(t1));
		queue->AddCommand(To_Command(t2));
		queue->AddCommand(To_Command(t3));
		queue->AddCommand(To_Command(t4));
		return queue;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5> shared_ptr<QueueCommand> q(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5)
	{
		shared_ptr<QueueCommand> queue(new QueueCommand);
		queue->AddCommand(To_Command(t1));
		queue->AddCommand(To_Command(t2));
		queue->AddCommand(To_Command(t3));
		queue->AddCommand(To_Command(t4));
		queue->AddCommand(To_Command(t5));
		return queue;
	}



	template <typename T1> shared_ptr<SimultaneusCommand> s(const T1 &t1)
	{
		shared_ptr<SimultaneusCommand> Simultaneus(new SimultaneusCommand);
		Simultaneus->AddCommand(To_Command(t1));
		return Simultaneus;
	}

	template <typename T1, typename T2> shared_ptr<SimultaneusCommand> s(const T1 &t1, const T2 &t2)
	{
		shared_ptr<SimultaneusCommand> Simultaneus(new SimultaneusCommand);
		Simultaneus->AddCommand(To_Command(t1));
		Simultaneus->AddCommand(To_Command(t2));
		return Simultaneus;
	}

	template <typename T1, typename T2, typename T3> shared_ptr<SimultaneusCommand> s(const T1 &t1, const T2 &t2, const T3 &t3)
	{
		shared_ptr<SimultaneusCommand> Simultaneus(new SimultaneusCommand);
		Simultaneus->AddCommand(To_Command(t1));
		Simultaneus->AddCommand(To_Command(t2));
		Simultaneus->AddCommand(To_Command(t3));
		return Simultaneus;
	}

	template <typename T1, typename T2, typename T3, typename T4> shared_ptr<SimultaneusCommand> s(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4)
	{
		shared_ptr<SimultaneusCommand> Simultaneus(new SimultaneusCommand);
		Simultaneus->AddCommand(To_Command(t1));
		Simultaneus->AddCommand(To_Command(t2));
		Simultaneus->AddCommand(To_Command(t3));
		Simultaneus->AddCommand(To_Command(t4));
		return Simultaneus;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5> shared_ptr<SimultaneusCommand> s(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5)
	{
		shared_ptr<SimultaneusCommand> Simultaneus(new SimultaneusCommand);
		Simultaneus->AddCommand(To_Command(t1));
		Simultaneus->AddCommand(To_Command(t2));
		Simultaneus->AddCommand(To_Command(t3));
		Simultaneus->AddCommand(To_Command(t4));
		Simultaneus->AddCommand(To_Command(t5));
		return Simultaneus;
	}



	class ScriptHolder : public boost::signals::trackable
	{
	public:
		ScriptHolder(const shared_ptr<Command> &command){ _command = command;};
		~ScriptHolder(){ }

		void operator () (Actor &actor)
		{ 
			if (!(*_command)(actor))
				delete this;
		}


	protected:
		shared_ptr<Command> _command;
	};
};

#endif