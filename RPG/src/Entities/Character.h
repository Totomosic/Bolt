#pragma once
#include "Entity.h"
#include "CharacterTextureSet.h"
#include "..\Tiles\MapTile.h"
#include "..\Spells\SpellManager.h"

namespace RPG
{

	class Character : public Entity
	{
	protected:
		CharacterOrientation m_Orientation;
		CharacterTextureSet m_TextureSet;
		float m_Speed;

		bool m_IsAttackable;
		bool m_ShowHealthbar;
		float m_MaxHealth;
		float m_CurrentHealth;
		GameObject* m_HealthbarObject;

		const MapTile* m_CurrentTile;
		const MapTile* m_TargetTile;

		SpellManager m_Spells;

	public:
		Character();
		Character(const MapTile* currentTile, const Vector2f& size, float speed, CharacterOrientation orientation, CharacterTextureSet textureSet, bool isAttackable = false, float maxHealth = 100, bool showHealthbar = true, id_t layerId = 0);

		CharacterOrientation Orientation() const;
		const CharacterTextureSet& TextureSet() const;
		CharacterTextureSet& TextureSet();
		const MapTile* CurrentTile() const;
		const MapTile* TargetTile() const;
		const SpellManager& Spells() const;
		SpellManager& Spells();

		float CurrentHealth() const;
		float MaxHealth() const;
		bool IsAttackable() const;
		bool IsShowingHealthbar() const;

		void SetOrientation(CharacterOrientation orientation);
		void Update() override;
		void SetCurrentTile(const MapTile* currentTile);
		void SetTargetTile(const MapTile* targetTile);

		void SetCurrentHealth(float health);
		void RemoveHealth(float health);
		void AddHealth(float health);
		void SetIsAttackable(bool attackable);
		void SetShowHealthbar(bool showHealthbar);

		GameObject* Create() override;
		void Destroy() override;

	protected:
		virtual GameObject* CreateHealthbar();
		virtual void DestroyHealthbar();
		virtual void ResizeHealthbar(GameObject* healthbar, float proportion);
		bool ShouldCreateHealthbar() const;
		bool ShouldDestroyHealthbar() const;

	public:
		static const Texture2D* GetTextureFromOrientation(CharacterOrientation orientation, const CharacterTextureSet& textureSet);
		static Vector2f GetDirectionFromOrientation(CharacterOrientation orientation);

	};

}