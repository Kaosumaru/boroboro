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
#include "collidable.h"
#include "GameResources.h"
#include "Items.h"

using namespace std;

namespace MX
{

class Player;

class PlayerSnake_Body : public Collidable
{
public:
	PlayerSnake_Body(ActorSprite *bef, Player *player, bool bAlternative);
	void sharpenTail();
	void newButt(ActorSprite *b);
	float GetSpeed();
	void Do();
	void onEat(Player* player);
	void Urwij();
	ActorSprite * GetButt() { return butt; }
	ActorSprite *before;  ///< before this segment
protected:

	friend class Player;
	Player* head;
	ActorSprite *butt;    ///< after this segment
	float speedMult;
	float dist;
	v2d toPos;
	v2d prevd;
	bool AlternativeLook;
	//unsigned num;
};

class Player : public Collidable
{
public:
	Player(const v2d& p, float d, bool alternative);

	void calculate_playerspeed();
	void KeyoardNavigate();
	void DrawItems();
	void Move();
	void AddBodypart();
	void Do();
	bool isInNeck(Collidable* c);
	void bounce(const v2d& normal);
	virtual void onEat(Player* another) override;

	inline float GetSpeed() { return speed * speed_multiplier; }

	float Rotation_Speed;
	float speed;
	float speed_multiplier;


	char KeyLeft;
	char KeyRight;
	char KeyUse;
	char KeyShield;

	ActorSprite *next_body_part;
	ActorSprite *last_body_part;

	shared_ptr<UseItem> Item;

	bool invisible;
	bool AlternativeLook;

	int GetLength() { return length; }
	void RecalcLength();

	v2d item_pos;
protected:
	int length;
	void ShieldSelf();
	EffectWithGivenCooldown Shield;

	unsigned headonColCounter;
	

	
};

};

void InitializeGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene);


void initGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene);
bool GameActive();

#endif