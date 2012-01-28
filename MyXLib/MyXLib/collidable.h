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
	void addCollidable(Collidable* c)
	{
		collidables.push_back(c);
	}

};

class Collidable : public MX::ActorSprite
{
public:
	virtual void onEat(MX::Player* player)
	{

	}
};