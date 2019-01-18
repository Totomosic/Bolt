#include "bltpch.h"
#include "SpellList.h"
#include "../GameManager.h"
#include "../Entities/TileMotion.h"
#include "../Networking/NetworkIdentity.h"

#include "Animators/FireballAnimator.h"

namespace DND
{

	void CreateBasicSpells(const ResourcePack& resources)
	{
		SpellList& spells = GameManager::Get().Spells();

		id_t explosionId = resources.GetResourceId("explosionAnimation");
		id_t fireballId = resources.GetResourceId("fireballAnimation");

		spells.AddSpell({ "Fireball", "", 0.5f, [](GameObject* caster, GameManager& gameManager)
		{
			OutputMemoryStream data;
			Serialize(data, gameManager.CurrentlySelectedTile());
			Serialize(data, Random::NextInt(20, 80));
			return std::move(data);
		},
		[explosionId, fireballId](GameObject* caster, InputMemoryStream& castData, GameManager& gameManager)
		{
			Tile selectedTile;
			int damage;
			Deserialize(castData, selectedTile);
			Deserialize(castData, damage);

			Vector3f position = gameManager.GetTilemap().WorldPositionOfTile(selectedTile.x, selectedTile.y);
			position.z += 1;
			GameObject* explosionShadow = gameManager.Network().Factory().Ellipse(gameManager.GetTilemap().TileWidth() * 0.75f, gameManager.GetTilemap().TileHeight() * 0.75f, Color(10, 10, 10, 200), Transform(position));

			Vector3f startPosition = caster->transform().Position() + Vector3f(0, 0, 1);
			float distance = (position.xy() - startPosition.xy()).Length();
			float speed = 1000;
			float time = distance / speed;
			GameObject* fireball = gameManager.Network().Factory().Image(300, 300, ResourceManager::Get<Texture2D>(fireballId), Transform());
			fireball->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(8, 1, 0.3f);
			fireball->Components().AddComponent<FireballAnimator>(startPosition, position, time, 100);

			Time::RenderingTimeline().AddFunction(time, [position, &gameManager, selectedTile, damage, explosionShadow, fireball, explosionId]()
			{
				Destroy(explosionShadow);
				Destroy(fireball);
				if (gameManager.Network().Server().IsRunning())
				{
					GameObject* explosion = gameManager.Network().Factory().Image(5 * gameManager.GetTilemap().TileWidth(), 5 * gameManager.GetTilemap().TileHeight(), ResourceManager::Get<Texture2D>(explosionId), Transform(position));
					explosion->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(9, 9, 1);
					Destroy(explosion, 1);
					GameObject* player = gameManager.LocalPlayer();
					Tile playerTile = player->Components().GetComponent<TileTransform>().CurrentTile();
					if (playerTile.x >= selectedTile.x - 2 && playerTile.x <= selectedTile.x + 2 && playerTile.y >= selectedTile.y - 2 && playerTile.y <= selectedTile.y + 2)
					{
						CharacterStats newStats = player->Components().GetComponent<StatsComponent>().Stats();
						newStats.CurrentHealth -= damage;
						player->Components().GetComponent<StatsComponent>().SetStats(newStats);
						StatUpdatePacket packet;
						packet.NetworkId = player->Components().GetComponent<NetworkIdentity>().NetworkId;
						packet.Stats = newStats;
						gameManager.Network().SendPacketToAll(packet);
					}
				}
			});

		}});

	}

}