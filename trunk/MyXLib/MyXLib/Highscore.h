#pragma once
#include <memory>
#include "../MXLib/MXAnimation.h"
#include "../MXLib/MXScene.h"
#include "Game.h"

extern void InitHighscore(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene, const shared_ptr<MX::Player> &player1, const shared_ptr<MX::Player> &player2);