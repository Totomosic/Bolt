#include "Character.h"
#include "..\GameManager.h"

namespace RPG
{

	Character::Character() : Entity(),
		m_Orientation(CharacterOrientation::Right), m_TextureSet(), m_Speed(0), m_CurrentTile(nullptr), m_TargetTile(nullptr), m_Spells(this)
	{

	}

	Character::Character(const MapTile* currentTile, const Vector2f& size, float speed, CharacterOrientation orientation, CharacterTextureSet textureSet, bool isAttackable, float maxHealth, bool showHealthbar, id_t layerId) : Entity(currentTile->Position(), size, GetTextureFromOrientation(orientation, textureSet), layerId),
		m_Orientation(orientation), m_TextureSet(textureSet), m_Speed(speed), m_IsAttackable(isAttackable), m_ShowHealthbar(showHealthbar), m_MaxHealth(maxHealth), m_CurrentHealth(maxHealth), m_CurrentTile(currentTile), m_TargetTile(nullptr),
		m_Spells(this)
	{
		if (m_CurrentTile != nullptr)
		{
			m_CurrentTile->SetObjectOnTile(this);
		}
	}

	CharacterOrientation Character::Orientation() const
	{
		return m_Orientation;
	}

	const CharacterTextureSet& Character::TextureSet() const
	{
		return m_TextureSet;
	}

	CharacterTextureSet& Character::TextureSet()
	{
		return m_TextureSet;
	}

	const MapTile* Character::CurrentTile() const
	{
		return m_CurrentTile;
	}

	const MapTile* Character::TargetTile() const
	{
		return m_TargetTile;
	}

	const SpellManager& Character::Spells() const
	{
		return m_Spells;
	}

	SpellManager& Character::Spells()
	{
		return m_Spells;
	}

	float Character::CurrentHealth() const
	{
		return m_CurrentHealth;
	}

	float Character::MaxHealth() const
	{
		return m_MaxHealth;
	}
	
	bool Character::IsAttackable() const
	{
		return m_IsAttackable;
	}

	bool Character::IsShowingHealthbar() const
	{
		return m_ShowHealthbar && m_IsAttackable;
	}

	void Character::SetOrientation(CharacterOrientation orientation)
	{
		m_Orientation = orientation;
		SetTexture(GetTextureFromOrientation(orientation, m_TextureSet));
	}

	void Character::Update()
	{
		if (m_TargetTile != m_CurrentTile && m_TargetTile != nullptr)
		{
			Vector2f toTarget = m_TargetTile->Position() - Position();
			Vector2f direction = toTarget.Normalize();
			Vector2f move = direction * min(m_Speed * Time::DeltaTime(), toTarget.Length());
			move /= Time::DeltaTime();
			m_Velocity = move;
		}
		else
		{
			m_Velocity = 0;
		}

		Entity::Update();
		CharacterOrientation newOrientation = m_Orientation;
		if (m_Velocity.x < 0)
		{
			newOrientation = CharacterOrientation::Left;
		}
		else if (m_Velocity.x > 0)
		{
			newOrientation = CharacterOrientation::Right;
		}
		else if (m_Velocity.y < 0)
		{
			newOrientation = CharacterOrientation::Down;
		}
		else if (m_Velocity.y > 0)
		{
			newOrientation = CharacterOrientation::Up;
		}
		if (newOrientation != m_Orientation)
		{
			SetOrientation(newOrientation);
		}

		if (m_TargetTile != m_CurrentTile && m_TargetTile != nullptr)
		{
			if (Vector2f::Distance(Position(), m_TargetTile->Position()) <= 0.001f)
			{
				m_CurrentTile->SetObjectOnTile(nullptr);
				m_CurrentTile = m_TargetTile;
				m_CurrentTile->SetObjectOnTile(this);
				m_TargetTile = nullptr;
			}
		}
	}

	void Character::SetCurrentTile(const MapTile* currentTile)
	{
		if (m_CurrentTile != nullptr)
		{
			m_CurrentTile->SetObjectOnTile(nullptr);
		}
		m_CurrentTile = currentTile;
		if (m_CurrentTile != nullptr)
		{
			m_CurrentTile->SetObjectOnTile(this);
		}
		SetPosition(currentTile->Position());
	}

	void Character::SetTargetTile(const MapTile* targetTile)
	{
		if (targetTile == nullptr || !targetTile->IsEnabled())
		{
			return;
		}
		m_TargetTile = targetTile;
		m_TargetTile->SetObjectOnTile(this);
	}

	void Character::SetCurrentHealth(float health)
	{
		m_CurrentHealth = max(health, 0.0f);
		if (IsShowingHealthbar())
		{
			ResizeHealthbar(m_HealthbarObject, m_CurrentHealth / m_MaxHealth);
		}
	}

	void Character::RemoveHealth(float health)
	{
		SetCurrentHealth(CurrentHealth() - health);
	}

	void Character::AddHealth(float health)
	{
		SetCurrentHealth(CurrentHealth() + health);
	}

	void Character::SetIsAttackable(bool isAttackable)
	{
		bool prev = m_IsAttackable;
		m_IsAttackable = isAttackable;
		if (prev != m_IsAttackable)
		{
			if (ShouldCreateHealthbar())
			{
				m_HealthbarObject = CreateHealthbar();
			}
			else if (ShouldDestroyHealthbar())
			{
				DestroyHealthbar();
			}
		}
	}

	void Character::SetShowHealthbar(bool showHealthbar)
	{
		bool prev = m_ShowHealthbar;
		m_ShowHealthbar = showHealthbar;
		if (prev != m_IsAttackable)
		{
			if (ShouldCreateHealthbar())
			{
				m_HealthbarObject = CreateHealthbar();
			}
			else if (ShouldDestroyHealthbar())
			{
				DestroyHealthbar();
			}
		}
	}

	GameObject* Character::Create()
	{
		GameObject* object = Entity::Create();
		Mesh& mesh = m_Object->Components().GetComponent<MeshRenderer>().Mesh;
		mesh.Models[0].Transform = Matrix4f::Translation(0, m_Size.y / 4.0f, 0) * mesh.Models[0].Transform;
		if (ShouldCreateHealthbar())
		{
			m_HealthbarObject = CreateHealthbar();
		}
		return object;
	}

	void Character::Destroy()
	{
		if (m_CurrentTile != nullptr)
		{
			m_CurrentTile->SetObjectOnTile(nullptr);
		}
		if (m_TargetTile != nullptr)
		{
			m_TargetTile->SetObjectOnTile(nullptr);
		}
		if (IsShowingHealthbar())
		{
			DestroyHealthbar();
		}
		Entity::Destroy();
	}

	GameObject* Character::CreateHealthbar()
	{
		GameObject* healthbar = GameObject::Instantiate(m_Object->GetLayer());
		healthbar->MakeChildOf(m_Object);
		healthbar->transform().SetLocalPosition(0, m_Size.y, 0);
		Mesh m;
		m.Models.push_back({ ResourceManager::Get<Model>("Sprite_Model"), Matrix4f::Translation(0, 0, 0) * Matrix4f::Scale(m_Size.x, 4, 1.0f),{ 0 } });
		m.Models.push_back({ ResourceManager::Get<Model>("Sprite_Model"), Matrix4f::Translation(0, 0, -1) * Matrix4f::Scale(m_Size.x, 4, 1.0f),{ 1 } });
		m.Materials[0].BaseColor = Color::Green;
		m.Materials[1].BaseColor = Color::Red;
		healthbar->Components().AddComponent(std::make_unique<MeshRenderer>(m));
		ResizeHealthbar(healthbar, m_CurrentHealth / m_MaxHealth);
		return healthbar;
	}

	void Character::DestroyHealthbar()
	{
		m_HealthbarObject->GetLayer()->RemoveGameObject(m_HealthbarObject);
		m_HealthbarObject = nullptr;
	}

	void Character::ResizeHealthbar(GameObject* healthbar, float proportion)
	{
		healthbar->Components().GetComponent<MeshRenderer>().Mesh.Models[0].Transform = Matrix4f::Translation(-(1.0f - proportion) / 2.0f * m_Size.x, 0, 0) * Matrix4f::Scale(proportion * m_Size.x, 4.0f, 1.0f);
	}

	bool Character::ShouldCreateHealthbar() const
	{
		return m_IsAttackable && m_ShowHealthbar;
	}

	bool Character::ShouldDestroyHealthbar() const
	{
		return !m_IsAttackable || !m_ShowHealthbar;
	}

	const Texture2D* Character::GetTextureFromOrientation(CharacterOrientation orientation, const CharacterTextureSet& textureSet)
	{
		switch (orientation)
		{
		case CharacterOrientation::Right:
			return textureSet.Right;
		case CharacterOrientation::Left:
			return textureSet.Left;
		case CharacterOrientation::Up:
			return textureSet.Up;
		case CharacterOrientation::Down:
			return textureSet.Down;
		default:
			return nullptr;
		}
	}

	Vector2f Character::GetDirectionFromOrientation(CharacterOrientation orientation)
	{
		switch (orientation)
		{
		case CharacterOrientation::Left:
			return Vector2f(-1, 0);
		case CharacterOrientation::Right:
			return Vector2f(1, 0);
		case CharacterOrientation::Up:
			return Vector2f(0, 1);
		case CharacterOrientation::Down:
			return Vector2f(0, -1);
		default:
			return Vector2f(0, 0);
		}
	}

}