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

	SpellInstance::SpellCastResult FireballInstance::Cast(GameObject* caster, const GameStateObjects& state)
	{
		BLT_INFO("FIREBALL DAMAGE {}", m_Damage.ToString());

		float distance = (m_Target.xy() - m_Start.xy()).Length();
		float time = distance / m_Speed;
		GameObject* fireball = state.Factory->Image(300, 300, state.Animations->Fireball.Animation, Transform());
		fireball->Components().AddComponent<SpriteAnimator>(state.Animations->Fireball.Animation);
		fireball->Components().GetComponent<SpriteAnimator>().PlayAnimationUntilStopped(state.Animations->Fireball, 0.3f);
		fireball->Components().AddComponent<FireballAnimator>(m_Start, m_Target, time, 100);
		fireball->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].RenderOptions.DepthFunc = DepthFunction::Lequal;

		m_Target.z -= 0.05f;
		GameObject* explosionShadow = state.Factory->Ellipse(state.MapManager->TileWidth() * 0.75f, state.MapManager->TileHeight() * 0.75f, Color(10, 10, 10, 200), Transform(m_Target));

		Vector3f target = m_Target;
		int damage = m_Damage.Result;

		Timer& createExplosion = Time::RenderingTimeline().AddFunction(time, [target, damage, explosionShadow, fireball, state]()
		{
			Destroy(explosionShadow);
			Destroy(fireball);
			if (state.Network->Server().IsRunning())
			{
				GameObject* explosion = state.Factory->Image(9 * state.MapManager->TileWidth(), 9 * state.MapManager->TileHeight(), state.Animations->FireballExplosion.Animation, Transform(target));
				explosion->Components().AddComponent<SpriteAnimator>(state.Animations->FireballExplosion.Animation);
				explosion->Components().GetComponent<SpriteAnimator>().PlayAnimationUntilStopped(state.Animations->FireballExplosion, 1.0f);
				explosion->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].RenderOptions.DepthFunc = DepthFunction::Lequal;
				Destroy(explosion, 1);
				GameObject* player = state.Players->LocalPlayerObject();
				Tile playerTile = player->Components().GetComponent<TileTransform>().CurrentTile();
				Tile selectedTile = state.MapManager->CurrentMap().TileFromWorldPosition(target.x, target.y);
				if (playerTile.x >= selectedTile.x - 3 && playerTile.x <= selectedTile.x + 3 && playerTile.y >= selectedTile.y - 3 && playerTile.y <= selectedTile.y + 3)
				{
					CharacterStats newStats = player->Components().GetComponent<StatsComponent>().Stats();
					newStats.CurrentHealth -= damage;
					player->Components().GetComponent<StatsComponent>().SetStats(newStats);
					StatsUpdatePacket packet;
					packet.NetworkId = player->Components().GetComponent<NetworkIdentity>().NetworkId;
					packet.NewStats = newStats;
					state.Network->SendPacketToAll(packet);
				}
			}
		});
		GameManager::Get().AddActiveTimer(&createExplosion);
		SpellInstance::SpellCastResult result;
		result.Cooldown = 2;
		result.CastTime = 0;
		result.Type = CastType::Action;
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

	FireballSpell::FireballSpell(int level) : Spell("Fireball", "", level)
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
		Vector3f target = state.Objects.MapManager->CurrentMap().WorldPositionOfTile(state.SelectedTile.x, state.SelectedTile.y);
		target.z = startPosition.z;
		float speed = 400;
		DiceRollResult damage = Dice::Roll(8, 6, 0);
		return std::make_unique<FireballInstance>(startPosition, target, speed, damage);
	}

}