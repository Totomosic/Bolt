#include "bltpch.h"
#include "FireballSpell.h"

#include "../../GameManager.h"
#include "../Animators/FireballAnimator.h"
#include "../../Entities/TileTransform.h"
#include "../../Networking/NetworkIdentity.h"

namespace DND
{

	FireballInstance::FireballInstance(const Vector3f& start, const Vector3f& target, float speed, const DiceRollResult& damage) : SpellInstance(),
		m_Start(start), m_Target(target), m_Speed(speed), m_Damage(damage)
	{
	
	}

	SpellInstance::SpellCastResult FireballInstance::Cast(Spell* spell, GameObject* caster, const GameStateObjects& state)
	{
		BLT_INFO("FIREBALL DAMAGE {}", m_Damage.ToString());
		FireballSpell* fireSpell = (FireballSpell*)spell;

		float distance = (m_Target.xy() - m_Start.xy()).Length();
		float time = distance / m_Speed;
		GameObject* fireball = state.Factory->Image(300, 300, ResourceManager::Get<Texture2D>(fireSpell->m_FireballAnimation), Transform());
		fireball->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(8, 1, 0.3f);
		fireball->Components().AddComponent<FireballAnimator>(m_Start, m_Target, time, 100);
		fireball->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].RenderOptions.DepthFunc = DepthFunction::Lequal;

		m_Target.z -= 0.05f;
		GameObject* explosionShadow = state.Factory->Ellipse(state.Map->TileWidth() * 0.75f, state.Map->TileHeight() * 0.75f, Color(10, 10, 10, 200), Transform(m_Target));

		Vector3f target = m_Target;
		int damage = m_Damage.Result;

		Timer& createExplosion = Time::RenderingTimeline().AddFunction(time, [target, damage, fireSpell, explosionShadow, fireball, state]()
		{
			Destroy(explosionShadow);
			Destroy(fireball);
			if (state.Network->Server().IsRunning())
			{
				GameObject* explosion = state.Factory->Image(6 * state.Map->TileWidth(), 6 * state.Map->TileHeight(), ResourceManager::Get<Texture2D>(fireSpell->m_ExplosionAnimation), Transform(target));
				explosion->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(9, 9, 1);
				explosion->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].RenderOptions.DepthFunc = DepthFunction::Lequal;
				Destroy(explosion, 1);
				GameObject* player = state.LocalPlayer;
				Tile playerTile = player->Components().GetComponent<TileTransform>().CurrentTile();
				Tile selectedTile = state.Map->TileFromWorldPosition(target.x, target.y);
				if (playerTile.x >= selectedTile.x - 2 && playerTile.x <= selectedTile.x + 2 && playerTile.y >= selectedTile.y - 2 && playerTile.y <= selectedTile.y + 2)
				{
					CharacterStats newStats = player->Components().GetComponent<StatsComponent>().Stats();
					newStats.CurrentHealth -= damage;
					player->Components().GetComponent<StatsComponent>().SetStats(newStats);
					StatUpdatePacket packet;
					packet.NetworkId = player->Components().GetComponent<NetworkIdentity>().NetworkId;
					packet.Stats = newStats;
					state.Network->SendPacketToAll(packet);
				}
			}
		});
		GameManager::Get().AddActiveFunction(&createExplosion);
		SpellInstance::SpellCastResult result;
		result.Cooldown = 2;
		return result;
	}

	OutputMemoryStream FireballInstance::SerializeInstance() const
	{
		OutputMemoryStream result;
		Serialize(result, m_Start);
		Serialize(result, m_Target);
		Serialize(result, m_Speed);
		Serialize(result, m_Damage);
		return result;
	}

	FireballSpell::FireballSpell(id_t explosionId, id_t fireballId) : Spell("Fireball", "", 3),
		m_ExplosionAnimation(explosionId), m_FireballAnimation(fireballId)
	{
	
	}

	std::unique_ptr<SpellInstance> FireballSpell::CreateInstance(InputMemoryStream& stream)
	{
		Vector3f start;
		Vector3f target;
		float speed;
		DiceRollResult damage;
		Deserialize(stream, start);
		Deserialize(stream, target);
		Deserialize(stream, speed);
		Deserialize(stream, damage);
		return std::make_unique<FireballInstance>(start, target, speed, damage);
	}

	std::unique_ptr<SpellInstance> FireballSpell::CreateInstance(GameObject* caster, const GameState& state)
	{
		Vector3f startPosition = caster->transform().Position() + Vector3f(0, 0, 1);
		Vector3f target = state.Objects.Map->WorldPositionOfTile(state.SelectedTile.x, state.SelectedTile.y);
		target.z = startPosition.z;
		float speed = 600;
		DiceRollResult damage = Dice::Roll(8, 6, 0);
		return std::make_unique<FireballInstance>(startPosition, target, speed, damage);
	}

}