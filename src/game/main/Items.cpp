#include "glm/vec2.hpp"
#include "Items.h"
#include "Player.h"
#include "Particles.h"
#include "scene/Script/SpriteCommands.h"
#include "scene/Script/CommonCommands.h"
#include "Game/Resources/Resources.h"
#include "graphic/images/TextureImage.h"
#include "utils/Random.h"

namespace Boro
{

void UseItem::Use(MX::SpriteScene *scene, Player *user)
{
	if (_soundUsed)
		_soundUsed->Play();
	onUse(scene, user);
}

GoodBootleItem::GoodBootleItem()
{
	item_image = MX::Resources::get().loadCenteredImage(32.0f, 32.0f, "images/bottle.png");
	static std::vector<MX::Sound::Sample::pointer> sounds = { MX::Resources::get().loadSound("sounds/sss1.ogg"),MX::Resources::get().loadSound("sounds/sss2.ogg"), MX::Resources::get().loadSound("sounds/sss3.ogg") };

	_soundUsed = Random::randomFrom(sounds);
	_soundPicked = MX::Resources::get().loadSound("sounds/gulp.ogg");
}

void GoodBootleItem::onUse(MX::SpriteScene *scene, Player *user)
{
	using namespace MX;

	user->speed_multiplier += 1.0f;
	auto command = command_from_functor([user]() 
	{ 
		user->speed_multiplier -= 1.0f; 
		return false; 
	});
	user->script.onRun.connect_command(q({ wait(2.0f), command }));
}


ShieldItem::ShieldItem()
{
	item_image = MX::Resources::get().loadCenteredImage(32.0f, 32.0f, "images/apple.png");
	_soundPicked = MX::Resources::get().loadSound("sounds/apple-crunch.ogg");
}

void ShieldItem::onUse(MX::SpriteScene *scene, Player *user)
{
	using namespace MX;

	user->speed_multiplier = 0.0f;
	user->invisible = true;
	auto command = command_from_functor([user]() 
	{ 
		user->speed_multiplier = 1.0f; 
		user->invisible = false;
		return false; 
	});
	user->script.onRun.connect_command(q({ wait(2.0f), command }));


	auto animation = MX::q({ lerp_color(0x80FFFFFF, 0.5f), wait(1.25f), lerp_color(0xFFFFFFFF, 0.25f) });
	user->script.onRun.connect_command(animation);

	for(auto next = dynamic_cast<PlayerSnake_Body*>(user->next_body_part);
		next != NULL;
		next = dynamic_cast<PlayerSnake_Body*>(next->GetButt()))
	{
		auto animation = MX::q({ lerp_color(0x80FFFFFF, 0.5f), wait(1.25f), lerp_color(0xFFFFFFFF, 0.25f) });
		next->script.onRun.connect_command(animation);
	}
}


PentagramItem::PentagramItem()
{
	item_image = MX::Resources::get().loadCenteredImage(32.0f, 32.0f, "images/PentaSmall.png");
	_soundUsed = MX::Resources::get().loadSound("sounds/flamewind2.ogg");
	_soundPicked = MX::Resources::get().loadSound("sounds/sssatan.ogg");
}

void PentagramItem::onUse(MX::SpriteScene *scene, Player *user)
{
	using namespace MX;

	AppendFlames(user->shared_from_this());
	
	user->pentakill = true;
	auto command = command_from_functor([user]() 
	{ 
		user->pentakill = false;
		return false; 
	});
	user->script.onRun.connect_command(q({ wait(2.0f), command }));
}





PoopItem::PoopItem()
{
	item_image = MX::Resources::get().loadCenteredImage(32.0f, 32.0f, "images/apple2.png");
	_soundUsed = MX::Resources::get().loadSound("sounds/shit.ogg");
	_soundPicked = MX::Resources::get().loadSound("sounds/apple-crunch.ogg");
}

void PoopItem::onUse(MX::SpriteScene *scene, Player *user)
{
	using namespace MX;
	class PoopMine : public MX::ImageSpriteAdapter<Collidable>
	{
	public:
		PoopMine(MX::SpriteScene *s, Player *user)
		{
			geometry.z = 0.9f;
			scene = s;
			script.onRun.connect_command(q({ wait(5.0f), lerp_color(0x00FFFFFF, 0.5f), unlink() }));

			_radius = 16.0f;
			geometry.position = user->geometry.position;

			auto next = dynamic_cast<PlayerSnake_Body*>(user->next_body_part);
			for(;next != NULL && next->GetButt() != NULL;next = dynamic_cast<PlayerSnake_Body*>(next->GetButt())){}

			if(next)
			{
				geometry.position = next->geometry.position;
			}

			auto image = MX::Resources::get().loadCenteredImage(32.0f, 32.0f, "images/shit.png");
			SetImage(image);
		}

		void onEat(Player* player)
		{
			if (!hit_by_poop.insert(player).second)
				return;

			player->speed_multiplier = 0.5f;
			auto command = command_from_functor([player]() 
			{ 
				player->speed_multiplier = 1.0f; 
				return false; 
			});
			player->script.onRun.connect_command(q({ wait(3.0f), command }));
		}

		std::set<Player*> hit_by_poop;
		MX::SpriteScene *scene;
	};


	auto command = command_from_functor([scene, user]() 
	{ 
		auto poop = std::make_shared<PoopMine>(scene, user);
		scene->AddActor(poop);
		return false; 
	});

	user->script.onRun.connect_command(l(5, { wait(.3f), command }));

}



}