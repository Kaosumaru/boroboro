#pragma once

#include <list>
#include "scene/sprites/ScriptableSpriteActor.h"

namespace Boro
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

	void allUCanEat(Boro::Player* player);
	bool doesCollide(Collidable* col);
};

class Collidable : public MX::ScriptableSpriteActor
{
public:
	float _radius;
	Collidable()
	{
		SphereWorld& sw = SphereWorld::getInst();
		_radius = 16.0f;
		sw.addCollidable(this);	
	}

	Collidable(MX::LScriptObject& script) : MX::ScriptableSpriteActor(script)
	{
		SphereWorld& sw = SphereWorld::getInst();
		_radius = 16.0f;
		script.load_property(_radius, "Radius");
		sw.addCollidable(this);	
	}

	virtual ~Collidable()
	{
		SphereWorld& sw = SphereWorld::getInst();
		sw.removeCollidable(this);
	}
	virtual void onEat(Boro::Player* player)
	{

	}
};