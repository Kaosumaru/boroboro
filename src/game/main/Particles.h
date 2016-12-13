#pragma once
#include <memory>
#include "Collidable.h"
#include "scene/sprites/SpriteScene.h"
#include "sound/Sample.h"
#include "graphic/images/Image.h"

using namespace std;

namespace Boro
{
	void AppendFrontBlood(const std::shared_ptr<MX::SpriteActor> &actor);
	void AppendBackBlood(const std::shared_ptr<MX::SpriteActor> &actor);
	void AppendFlames(const std::shared_ptr<MX::SpriteActor> &actor);
}
