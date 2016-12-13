#pragma once
#include <memory>
#include "Collidable.h"
#include "scene/sprites/SpriteScene.h"
#include "sound/Sample.h"
#include "graphic/images/Image.h"

using namespace std;

namespace Boro
{
	class UseItem
	{
	public:
		virtual ~UseItem(){};

		virtual void Use(MX::SpriteScene *scene, Player *user);

		std::shared_ptr<MX::Graphic::Image> item_image;

		void onPickedUp()
		{
			if (_soundPicked)
				_soundPicked->Play();
		}
	protected:
		std::shared_ptr<MX::Sound::Sample> _soundUsed;
		std::shared_ptr<MX::Sound::Sample> _soundPicked;
		virtual void onUse(MX::SpriteScene *scene, Player *user) = 0;
	};

	class GoodBootleItem : public UseItem
	{
	public:
		GoodBootleItem();
		void onUse(MX::SpriteScene *scene, Player *user) override;
	};

	class ShieldItem : public UseItem
	{
	public:
		ShieldItem();
		void onUse(MX::SpriteScene *scene, Player *user) override;
	};

	class PentagramItem : public UseItem
	{
	public:
		PentagramItem();
		void onUse(MX::SpriteScene *scene, Player *user) override;
	};

	class PoopItem : public UseItem
	{
	public:
		PoopItem();
		void onUse(MX::SpriteScene *scene, Player *user) override;
	};
}
