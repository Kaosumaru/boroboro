#include "glm/vec2.hpp"
#include "Pickups.h"
#include "Player.h"
#include "script/PropertyLoaders.h"
#include "Game/Resources/Resources.h"
#include "scene/Script/SpriteCommands.h"
#include "scene/Script/CommonCommands.h"
#include "scene/generators/ActorFactory.h"
#include "scene/generators/ActorGenerator.h"

namespace Boro
{
	class Stone1 : public MX::ImageSpriteAdapter<Collidable>
	{
	public:
		Stone1(const glm::vec2& pos)
		{
			geometry.position = pos;
			geometry.z = 1.0f;
			geometry.scale = { 0.5f, 0.5f };

			auto image = MX::Resources::get().loadCenteredImage(64.0f, 64.0f, "images/kamien.png");
			SetImage(image);
		}

		void onEat(Player* player) override
		{
			auto normal = glm::normalize(player->geometry.position - geometry.position);
			player->bounce(normal);
			return;
		}
	};

	class Pickup : public MX::ImageSpriteAdapter<Collidable>
	{
	protected:
		void onEat(Player* player) override
		{
			if (_pickedUp)
				_pickedUp->Play();
			Unlink();
		}

		std::shared_ptr<MX::Sound::Sample> _pickedUp;
	};

	class Berry : public Pickup
	{
	public:
		Berry()
		{
			geometry.z = 0.9f;

			geometry.position.x = (float)(rand()%1280);
			geometry.position.y = (float)(rand()%800);

			_pickedUp = MX::Resources::get().loadSound("sounds/apple-crunch.ogg");
			auto image = MX::Resources::get().loadCenteredImage(64.0f, 64.0f, "images/berry.png");
			SetImage(image);

			script.onRun.connect_command(MX::q({ MX::wait(5.0f), MX::lerp_color({1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, 1.0f), MX::unlink() }));
		}

		void onEat(Player* player) override
		{
			player->AddBodypart(3);
			Pickup::onEat(player);			
		}
	};

	class BerrySpawner : public MX::ActorFactory
	{
	public:
		BerrySpawner()
		{
			auto time = std::make_pair(3.0f, 5.0f);
			SetGenerator(std::make_shared<MX::ActorGeneratorRandomIntervals>(time));
		}

		std::shared_ptr<ScriptableSpriteActor> generateActor() override
		{
			return std::make_shared<Berry>();
		}
	};


	class ItemPickup : public Pickup
	{
	public:
        using Creator = std::function<std::shared_ptr<UseItem>()>;

        ItemPickup( const Creator& creator, const std::string& image, const std::string& sound )
		{
            _creator = creator;
			geometry.z = 0.9f;
			_pickedUp = MX::Resources::get().loadSound(sound);
			SetImage(MX::Resources::get().loadCenteredImage(64.0f, 64.0f, image));

			script.onRun.connect_command(MX::q({ MX::wait(5.0f), MX::lerp_color({1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, 3.0f), MX::unlink() }));
		}

		void onEat(Player* player) override
		{
            if (_creator)
                player->GainItem(_creator());
			Pickup::onEat(player);			
		}
    protected:
        Creator _creator;
	};

	class ItemPickupSpawner : public MX::ActorFactory
	{
	public:
		ItemPickupSpawner()
		{
			auto time = std::make_pair(20.0f, 25.0f);
			SetGenerator(std::make_shared<MX::ActorGeneratorRandomIntervals>(time));
		}

		std::shared_ptr<ScriptableSpriteActor> generateActor() override
		{
            float x_rand = 50.0f + (float)(rand()%1180); //WIPMAGIC
            float y_rand = 50.0f + (float)(rand()%700); //WIPMAGIC
            std::shared_ptr<ItemPickup> item;
		    switch (rand() % 10)
		    {
		    case 0:
		    case 1:
		    case 2:
			    //pItem = make_shared<BonusItem<GoodBootleItem>>(&SoundBank::gulp, GraphicRes.bottle, 5000, 3000);
			    break;
		    case 3:
		    case 4:
		    case 5:
			    //pItem = make_shared<BonusItem<PoopItem>>(&SoundBank::apple_bite, GraphicRes.rotten_apple, 5000, 3000);
			    break;
		    case 6:
		    case 7:
		    case 8:			
			    //pItem = make_shared<BonusItem<ShieldItem>>(&SoundBank::apple_bite, GraphicRes.shield, 5000, 3000);
			    break;
		    case 9:
			    //pItem = make_shared<PentagramBonus>();
    			    break;
            }

            if ( !item )
                return item;

            item->geometry.position = { x_rand, y_rand };

		    int nTries = 3;
		    while (SphereWorld::getInst().doesCollide(item.get()))
		    {
			    x_rand = (float)(rand()%1280);
			    y_rand = (float)(rand()%800);

			    item->geometry.position = { x_rand, y_rand };

			    if (--nTries == 0)
				    return nullptr;
            }
            return item;
		}
	};


	void InitLevel(MX::SpriteScene* scene)
	{
		scene->AddActor(make_shared<Stone1>(glm::vec2{ 600.0f, 600.0f }));

		for (int i = 0; i < 5; i ++)
			scene->AddActor(make_shared<Stone1>(glm::vec2{ (float)(rand() % 1280), (float)(rand() % 800) }));

		scene->AddActor(make_shared<BerrySpawner>());
        scene->AddActor(make_shared<ItemPickupSpawner>());
	}

}