#ifndef ORZEL1_GAME
#define ORZEL1_GAME
#include <memory>
#include "../MXLib/MXDraw.h"
#include "../MXLib/MXSpriter.h"
#include "../MXLib/MXWorld.h"
#include "../MXLib/MXAnimation.h"
#include "../MXLib/MXSound.h"
#include "../MXLib/MXFont.h"
#include "../MXLib/MXLine.h"
#include "../MXLib/MXScript.h"


using namespace std;

void InitializeGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene);


void initGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene);
bool GameActive();

#endif