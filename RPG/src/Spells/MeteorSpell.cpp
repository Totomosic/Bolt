#include "MeteorSpell.h"
#include "..\Entities\Character.h"
#include "..\Tiles\MapTile.h"

namespace RPG
{

	MeteorSpell::MeteorSpell(float cooldown, float cost, float baseDamage, float damageScaling, float outerScaling, const Vector2f& size, const Texture2D* texture) : Spell(cooldown, cost),
		m_BaseDamage(baseDamage), m_Scaling(damageScaling), m_OuterDamageProp(outerScaling), m_Size(size), m_Texture(texture)
	{
	
	}

	void MeteorSpell::Cast(Character* caster, const MapTile* selectedTile, const Character* selectedCharacter)
	{
		// SPAWN ON PLAYER?? FLING IN ARC TO POSITION
		GameObject* meteor = GameObject::Instantiate(caster->Object()->GetLayer());
		Mesh mesh;
		mesh.Models.push_back({ ResourceManager::Get<Model>("Sprite_Model"), Matrix4f::Scale(m_Size.x, m_Size.y, 1.0f), { 0 } });
		mesh.Materials[0].Textures.Textures.push_back(m_Texture);
		mesh.Materials[0].Shader = Shader::DefaultTexture();
		mesh.Materials[0].BaseColor = Color(50, 50, 50, 255);
		meteor->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));

		Vector2f startPosition = Vector2f(caster->Position().x, caster->Position().y + caster->Size().y + m_Size.y / 2);
		Vector2f endPosition = selectedTile->Position();
		float time = 0.5f;
		meteor->transform().SetLocalPosition(startPosition.x, startPosition.y, -50);
		meteor->Components().AddComponent(std::make_unique<MotionEngine>((endPosition - startPosition) / time, Vector3f(0, 0, 0)));
		Destroy(meteor, time);
	}

}