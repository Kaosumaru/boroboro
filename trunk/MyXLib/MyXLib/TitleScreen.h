#pragma once

#include <vector>
#include "../MXLib/MXDraw.h"
#include "../MXLib/MXScene.h"
#include "../MXLib/MXSpriter.h"
#include "GameResources.h"


namespace MX
{

void InitializeTitle(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene);

};