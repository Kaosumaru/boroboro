#pragma once

#include <vector>
#include "../MXLib/MXScene.h"
#include "Game.h"

class Collidable;

class SphereWorld
{
	typedef std::vector<Collidable*> ColCol;  // Coladibles Collection
	ColCol collidables;
public:
	addCollidable(Colidable* c)
	{
		collidables.push_back(c);
	}

};

class Collidable : public ActorSprite
{
public:
	virtual void onEat(Player* player)
	{

	}
};