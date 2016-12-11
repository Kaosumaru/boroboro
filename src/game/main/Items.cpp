#include "glm/vec2.hpp"
#include "Items.h"
#include "Player.h"
#include "scene/Script/SpriteCommands.h"
#include "scene/Script/CommonCommands.h"
#include "Game/Resources/Resources.h"

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
	//item_image = MX::Resources::get().loadImage();
	_soundUsed = MX::Resources::get().loadSound("sounds/gulp.ogg");
}

void GoodBootleItem::onUse(MX::SpriteScene *scene, Player *user)
{
	using namespace MX;

	user->speed_multiplier += 2.0f;
	auto command = command_from_functor([user]() 
	{ 
		user->speed_multiplier -= 2.0f; 
		return false; 
	});
	user->script.onRun.connect_command(q({ wait(2.0f), command }));
}


ShieldItem::ShieldItem()
{
	_soundUsed = MX::Resources::get().loadSound("sounds/gulp.ogg");
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
	_soundUsed = MX::Resources::get().loadSound("sounds/sssatan.ogg");
}

void PentagramItem::onUse(MX::SpriteScene *scene, Player *user)
{
	using namespace MX;

#ifdef WIP
	//fire particle
#endif

	user->pentakill = true;
	auto command = command_from_functor([user]() 
	{ 
		user->pentakill = false;
		return false; 
	});
	user->script.onRun.connect_command(q({ wait(2.0f), command }));
}


}