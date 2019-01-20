#include "bltpch.h"
#include "Dice.h"
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
		id_t fireballIcon = resources.GetResourceId("fireballIcon");

		id_t teleportIcon = resources.GetResourceId("teleportIcon");

		spells.AddSpell({ "Fireball", "", 0.5f, 2.0f, 3, fireballIcon, [](GameObject* caster, const GameState& state)
		{
			DiceRollResult damage = Dice::Roll(8, 6, 0);
			OutputMemoryStream data;
			Serialize(data, state.SelectedTile);
			Serialize(data, damage);
			return std::move(data);
		},
		[explosionId, fireballId](GameObject* caster, InputMemoryStream& castData, const GameStateObjects& state)
		{
			Tile selectedTile;
			DiceRollResult damage;
			Deserialize(castData, selectedTile);
			Deserialize(castData, damage);
			BLT_CORE_INFO("FIREBALL DAMAGE {}", damage.ToString());

			Vector3f position = state.Map->WorldPositionOfTile(selectedTile.x, selectedTile.y);
			position.z += 1;

			Vector3f startPosition = caster->transform().Position() + Vector3f(0, 0, 1);
			float distance = (position.xy() - startPosition.xy()).Length();
			float speed = 600;
			float time = distance / speed;
			GameObject* fireball = state.Factory->Image(300, 300, ResourceManager::Get<Texture2D>(fireballId), Transform());
			fireball->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(8, 1, 0.3f);
			fireball->Components().AddComponent<FireballAnimator>(startPosition, position, time, 100);
			fireball->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].RenderOptions.DepthFunc = DepthFunction::Lequal;

			position.z -= 0.05f;
			GameObject* explosionShadow = state.Factory->Ellipse(state.Map->TileWidth() * 0.75f, state.Map->TileHeight() * 0.75f, Color(10, 10, 10, 200), Transform(position));

			Timer& createExplosion = Time::RenderingTimeline().AddFunction(time, [position, selectedTile, damage, explosionShadow, fireball, explosionId, state]()
			{
				Destroy(explosionShadow);
				Destroy(fireball);
				if (state.Network->Server().IsRunning())
				{
					GameObject* explosion = state.Factory->Image(6 * state.Map->TileWidth(), 6 * state.Map->TileHeight(), ResourceManager::Get<Texture2D>(explosionId), Transform(position));
					explosion->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(9, 9, 1);
					explosion->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].RenderOptions.DepthFunc = DepthFunction::Lequal;
					Destroy(explosion, 1);
					GameObject* player = state.LocalPlayer;
					Tile playerTile = player->Components().GetComponent<TileTransform>().CurrentTile();
					if (playerTile.x >= selectedTile.x - 2 && playerTile.x <= selectedTile.x + 2 && playerTile.y >= selectedTile.y - 2 && playerTile.y <= selectedTile.y + 2)
					{
						CharacterStats newStats = player->Components().GetComponent<StatsComponent>().Stats();
						newStats.CurrentHealth -= damage.Result;
						player->Components().GetComponent<StatsComponent>().SetStats(newStats);
						StatUpdatePacket packet;
						packet.NetworkId = player->Components().GetComponent<NetworkIdentity>().NetworkId;
						packet.Stats = newStats;
						state.Network->SendPacketToAll(packet);
					}
				}
			});
			GameManager::Get().AddActiveFunction(&createExplosion);
		}});



		spells.AddSpell({ "Teleport", "", 0.5f, 5.0f, 2, teleportIcon, [](GameObject* caster, const GameState& state)
		{
			OutputMemoryStream castData;
			Serialize(castData, state.SelectedTile);
			return std::move(castData);
		},
		[](GameObject* caster, InputMemoryStream& castData, const GameStateObjects& state)
		{
			Tile selectedTile;
			Deserialize(castData, selectedTile);
			Timer& tpFunc = Time::RenderingTimeline().AddFunction(0.25, [caster, selectedTile]()
			{
				caster->Components().GetComponent<TileTransform>().SetCurrentTile(selectedTile, true);
				caster->Components().GetComponent<TileMotion>().SetTargetTile(selectedTile);
			});
			GameManager::Get().AddActiveFunction(&tpFunc);
		}});

	}

}