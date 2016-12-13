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

			script.onRun.connect_command(MX::q({ MX::wait(5.0f), MX::lerp_color({1.0f, 1.0f, 1.0f, 0.0f}, 1.0f), MX::unlink() }));
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
        ItemPickup( const std::shared_ptr<UseItem>& item )
		{
			_item = item;
			geometry.z = 0.9f;
			SetImage(item ? item->item_image : 0);
			script.onRun.connect_command(MX::q({ MX::wait(5.0f), MX::lerp_color({1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, 3.0f), MX::unlink() }));
		}

		void onEat(Player* player) override
		{
			if (_item)
			{
				_item->onPickedUp();
				player->GainItem(_item);
			}
			Pickup::onEat(player);			
		}
    protected:
		std::shared_ptr<UseItem> _item;
	};

	class ItemPickupSpawner : public MX::ActorFactory
	{
	public:
		ItemPickupSpawner()
		{
			//auto time = std::make_pair(20.0f, 25.0f);
			auto time = std::make_pair(5.0f, 7.0f);
			SetGenerator(std::make_shared<MX::ActorGeneratorRandomIntervals>(time));
		}

		std::shared_ptr<ScriptableSpriteActor> generateActor() override
		{
            float x_rand = 50.0f + (float)(rand()%1180); //WIPMAGIC
            float y_rand = 50.0f + (float)(rand()%700); //WIPMAGIC
            
			std::shared_ptr<UseItem> item;
		    switch (rand() % 10)
		    {
		    case 0:
		    case 1:
		    case 2:
				item = std::make_shared<GoodBootleItem>();
			    break;
		    case 3:
		    case 4:
		    case 5:
				item = std::make_shared<PoopItem>();
			    break;
		    case 6:
		    case 7:
		    case 8:			
				item = std::make_shared<ShieldItem>();
			    break;
		    case 9:
				item = std::make_shared<PentagramItem>();
    			break;
            }

            if ( !item )
                return nullptr;

			std::shared_ptr<ItemPickup> pickup = std::make_shared<ItemPickup>(item);

			pickup->geometry.position = { x_rand, y_rand };

		    int nTries = 3;
		    while (SphereWorld::getInst().doesCollide(pickup.get()))
		    {
			    x_rand = (float)(rand()%1280);
			    y_rand = (float)(rand()%800);

				pickup->geometry.position = { x_rand, y_rand };

			    if (--nTries == 0)
				    return nullptr;
            }
            return pickup;
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