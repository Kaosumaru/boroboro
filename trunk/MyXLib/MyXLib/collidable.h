#pragma once

#include <vector>
#include "../MXLib/MXScene.h"

class SphereWorld
{
	std::vector<Collidable*> collidables;
public:
	addCollidable(Colidable* c)
	{
		//co
	}

};

class Collidable : public ActorSprite
{
public:
	virtual void onEat(Player* player)
	{

	}
};