#pragma once

#include <list>
#include "../MXLib/MXScene.h"

namespace MX
{
class Player;

class UseItem
{
public:
	virtual ~UseItem(){};

	virtual void Use(Scene *scene, Player *user) = 0;
};

class GoodBootleItem : public UseItem
{
public:
	void Use(Scene *scene, Player *user);
};

};