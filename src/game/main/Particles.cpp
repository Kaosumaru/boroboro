#include "Particles.h"
#include "Items.h"
#include "Player.h"
#include "scene/Script/SpriteCommands.h"
#include "scene/Script/CommonCommands.h"
#include "Game/Resources/Resources.h"
#include "graphic/images/TextureImage.h"
#include "utils/Random.h"
#include "Script/ScriptObject.h"
#include "scene/Script/SpriteCommands.h"
#include "script/PropertyLoaders.h"

namespace Boro
{
	void AppendFrontBlood(const std::shared_ptr<MX::SpriteActor> &actor)
	{
		MX::ScriptObjectString script("Game.Res");

		static std::shared_ptr<MX::ScriptableSpriteActor> sparticle;

		if (!sparticle)
			script.load_property(sparticle, "FrontBlood");

		if (!sparticle)
			return;

		auto particle = sparticle->cloneSprite();

		auto &scene = actor->sprite_scene();
		particle->script.onRun.connect_command(glue_to(actor));
		scene.AddActor(particle);
	}

	void AppendBackBlood(const std::shared_ptr<MX::SpriteActor> &actor)
	{
		MX::ScriptObjectString script("Game.Res");

		static std::shared_ptr<MX::ScriptableSpriteActor> sparticle;

		if (!sparticle)
			script.load_property(sparticle, "BackBlood");

		if (!sparticle)
			return;

		auto particle = sparticle->cloneSprite();

		auto &scene = actor->sprite_scene();
		particle->script.onRun.connect_command(glue_to(actor));
		scene.AddActor(particle);
	}

	void AppendFlames(const std::shared_ptr<MX::SpriteActor> &actor)
	{
		MX::ScriptObjectString script("Game.Res");

		static std::shared_ptr<MX::ScriptableSpriteActor> sparticle;

		if (!sparticle)
			script.load_property(sparticle, "Flames");

		if (!sparticle)
			return;

		auto particle = sparticle->cloneSprite();

		auto &scene = actor->sprite_scene();
		particle->script.onRun.connect_command(glue_to(actor));
		scene.AddActor(particle);
	}

}