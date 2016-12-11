#pragma once
#include <memory>
#include "Collidable.h"
#include "Controller.h"
#include "sound/Sample.h"

using namespace std;

namespace Boro
{

class Player;

class PlayerSnake_Body : public MX::ImageSpriteAdapter<Collidable>
{
public:
	PlayerSnake_Body(SpriteActor *bef, Player *player, const std::shared_ptr<MX::Graphic::Image> &image);
	void sharpenTail();
	void newButt(SpriteActor *b);
	float GetSpeed();
	void Run() override;
	void Draw(float x, float y);
	void onEat(Player* player);
	void Urwij();
	SpriteActor * GetButt() { return butt; }
	SpriteActor *before;  ///< before this segment
protected:

	friend class Player;
	Player* head;
	Player* owner;
	SpriteActor *butt;    ///< after this segment
	float speedMult = 1.0f;
	float dist;
	glm::vec2 toPos;
	glm::vec2 prevd;
	//unsigned num;
};

struct World_Tag {};

class Player : public MX::AnimatedSpriteAdapter<Collidable>
{
	friend class PlayerSnake_Body;
public:
	typedef enum
	{
		NormalControl = 0,
		ForceLeft,
		ForceRight
	} MovementType;

	Player(MX::LScriptObject& script);

	void calculate_playerspeed();
	void Navigate();
	void DrawItems();
	void Move();
	void AddBodypart();
	void Run() override;
	bool isInNeck(Collidable* c);
	void bounce(const glm::vec2& normal);
	virtual void onEat(Player* another) override;

	inline float GetSpeed() { return speed * speed_multiplier; }

	float Rotation_Speed = 2.0f;
	float speed = 150.0f;
	float speed_multiplier = 1.0f;
	float rotation_proportion = 75.0f;

	SpriteActor *next_body_part = nullptr;
	SpriteActor *last_body_part = this;

#ifdef WIP
	shared_ptr<UseItem> Item;
#endif

	bool invisible = false;
	bool pentakill = false;

	int GetLength() { return length; }
	void RecalcLength();

	glm::vec2 item_pos;
	MovementType MovType;

	int score = 0;
protected:
	auto& world()
	{
		return  Context<MX::SpriteScene, Boro::World_Tag>::current();
	}

	std::shared_ptr<Controller> _controller;
	std::shared_ptr<MX::Graphic::Image> _body;

	int length = 0;
	unsigned headonColCounter = 0;


	std::shared_ptr<MX::Sound::Sample> _noAss;
	std::shared_ptr<MX::Sound::Sample> _bite;
	std::shared_ptr<MX::Sound::Sample> _bump;

#ifdef WIP
	EffectWithGivenCooldown Shield{10000};
#endif	
};

};

#ifdef WIP
void InitializeGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene);
void EndGame();

void InitializeDemo(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene);


void initGame(const shared_ptr<MX::Draw> &_draw, const shared_ptr<MX::Spriter> &_spriter, MX::Scene *_scene);
bool GameActive();

#endif