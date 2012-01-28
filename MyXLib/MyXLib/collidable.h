#pragma once

#include <list>
#include "../MXLib/MXScene.h"

namespace MX
{
class Player;
}
class Collidable;

class SphereWorld
{
	typedef std::list<Collidable*> ColCol;  // Coladibles Collection
	ColCol collidables;
	SphereWorld()
	{
	}
public:
	void addCollidable(Collidable* c)
	{
		collidables.push_back(c);
	}

	void removeCollidable(Collidable* c)
	{
		collidables.remove(c);
	}
	static SphereWorld& getInst()
	{
		static SphereWorld sw;
		return sw;
	}

	void allUCanEat(MX::Player* player);
};

extern SphereWorld& sw;

class Collidable : public MX::ActorSprite
{
public:
	float r;
	Collidable()
	{
		r = 16.0f;
		sw.addCollidable(this);	
	}
	virtual ~Collidable()
	{
		sw.removeCollidable(this);
	}
	virtual void onEat(MX::Player* player)
	{

	}
};