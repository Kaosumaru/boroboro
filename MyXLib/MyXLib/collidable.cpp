#include "stdafx.h"
#include "collidable.h"
#include "Game.h"

SphereWorld& sw = SphereWorld::getInst();

void SphereWorld::allUCanEat(MX::Player* player)
{
	for(auto it = collidables.begin(); it!=collidables.end(); ++it)
	{
		if (!(*it)->alive)
			continue;
		if(player->isInNeck(*it) || *it == player)
			continue;
		v2d d = (*it)->pos - player->pos;
		float rrit = (*it)->r; //(*it)->scaleX*(*it)->scaleX + (*it)->scaleY*(*it)->scaleY;
		float rrp = player->r; //player->scaleX*player->scaleX + player->scaleY*player->scaleY;
		if(norm(d) < rrit*rrit + rrp*rrp + 2*rrit*rrp)
			(*it)->onEat(player);
	}

	static const float MARGIN = 50.0f;

	if(player->pos.x < MARGIN)
		player->bounce(v2d(1.0f, 0.0f));
	else if(player->pos.x > 1280-MARGIN)
		player->bounce(v2d(-1.0f, 0.0f));

	if(player->pos.y < MARGIN)
		player->bounce(v2d(0.0f, 1.0f));
	else if(player->pos.y > 800-MARGIN)
		player->bounce(v2d(0.0f, -1.0f));
}


bool SphereWorld::doesCollide(Collidable* col)
{
	for(auto it = collidables.begin(); it!=collidables.end(); ++it)
	{
		if (!(*it)->alive)
			continue;
		if(*it == col)
			continue;
		v2d d = (*it)->pos - col->pos;
		float rrit = (*it)->r; //(*it)->scaleX*(*it)->scaleX + (*it)->scaleY*(*it)->scaleY;
		float rrp = col->r; //player->scaleX*player->scaleX + player->scaleY*player->scaleY;
		if(norm(d) < rrit*rrit + rrp*rrp + 2*rrit*rrp)
			return true;
	}
	return false;
}