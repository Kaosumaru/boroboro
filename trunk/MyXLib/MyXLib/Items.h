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

	std::shared_ptr<MX::Animation> item_image;
};

class GoodBootleItem : public UseItem
{
public:
	GoodBootleItem();
	void Use(Scene *scene, Player *user);
};


class ShieldItem : public UseItem
{
public:
	ShieldItem();
	void Use(Scene *scene, Player *user);
};

class PoopItem : public UseItem
{
public:
	PoopItem();
	void Use(Scene *scene, Player *user);
};

class PentagramItem : public UseItem
{
public:
	PentagramItem();
	void Use(Scene *scene, Player *user);
};


};