#include "stdafx.h"
#include "collidable.h"
#include "Game.h"

SphereWorld& sw = SphereWorld::getInst();

void SphereWorld::allUCanEat(MX::Player* player)
{
	for(auto it = collidables.begin(); it!=collidables.end(); ++it)
	{
		v2d d = (*it)->pos - player->pos;
		float rrit = (*it)->r; //(*it)->scaleX*(*it)->scaleX + (*it)->scaleY*(*it)->scaleY;
		float rrp = player->r; //player->scaleX*player->scaleX + player->scaleY*player->scaleY;
		if(norm(d) < rrit*rrit + rrp*rrp + 2*rrit*rrp)
			(*it)->onEat(player);
	}
}