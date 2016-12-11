#include "collidable.h"
#include "Player.h"
#include "glm/vec2.hpp"

void SphereWorld::allUCanEat(Boro::Player* player)
{
#ifdef WIP
	for(auto it = collidables.begin(); it!=collidables.end(); ++it)
	{
		if (!(*it)->alive)
			continue;
		if(player->isInNeck(*it) || *it == player)
			continue;
		glm::vec2 d = (*it)->pos - player->pos;
		float rrit = (*it)->_radius; //(*it)->scaleX*(*it)->scaleX + (*it)->scaleY*(*it)->scaleY;
		float rrp = player->_radius; //player->scaleX*player->scaleX + player->scaleY*player->scaleY;
		if(norm(d) < rrit*rrit + rrp*rrp + 2*rrit*rrp)
			(*it)->onEat(player);
	}

#endif
	static const float MARGIN = 50.0f;
	static const glm::vec2 bounds = { 1280.0f, 800.0f };

	if(player->geometry.position.x < MARGIN)
		player->bounce(glm::vec2(1.0f, 0.0f));
	else if(player->geometry.position.x > bounds.x-MARGIN)
		player->bounce(glm::vec2(-1.0f, 0.0f));

	if(player->geometry.position.y < MARGIN)
		player->bounce(glm::vec2(0.0f, 1.0f));
	else if(player->geometry.position.y > bounds.y-MARGIN)
		player->bounce(glm::vec2(0.0f, -1.0f));
}


bool SphereWorld::doesCollide(Collidable* col)
{
	for(auto it = collidables.begin(); it!=collidables.end(); ++it)
	{
#ifdef WIP
		if (!(*it)->alive)
			continue;
		if(*it == col)
			continue;
		glm::vec2 d = (*it)->pos - col->pos;
		float rrit = (*it)->r; //(*it)->scaleX*(*it)->scaleX + (*it)->scaleY*(*it)->scaleY;
		float rrp = col->r; //player->scaleX*player->scaleX + player->scaleY*player->scaleY;
		if(norm(d) < rrit*rrit + rrp*rrp + 2*rrit*rrp)
			return true;
#endif
	}
	return false;
}