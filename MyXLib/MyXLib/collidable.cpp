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

	if(player->pos.x < 30)
		player->bounce(v2d(1.0f, 0.0f));
	else if(player->pos.x > 1250)
		player->bounce(v2d(-1.0f, 0.0f));

	if(player->pos.y < 30)
		player->bounce(v2d(0.0f, 1.0f));
	else if(player->pos.y > 770)
		player->bounce(v2d(0.0f, -1.0f));
}