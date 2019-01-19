#include "bltpch.h"
#include "SpellCooldownMenu.h"
#include "../GameManager.h"
#include "../Entities/PlayerController.h"

namespace DND
{

	SpellCooldownMenu::SpellCooldownMenu(Layer* layer, const Vector3f& position, float width, float height, SpellManager* manager) : UImenu(),
		m_Factory(*layer), m_SpellIcons(), m_Manager(manager), m_MenuPosition(position), m_MenuWidth(width), m_MenuHeight(height)
	{
		
	}

	void SpellCooldownMenu::CreateMenu()
	{
		if (m_Manager == nullptr)
		{
			m_Manager = &GameManager::Get().LocalPlayer()->Components().GetComponent<PlayerController>().Spells();
		}
		GameObject* root = m_Factory.Instantiate(Transform(m_MenuPosition));
		m_Factory.SetCurrentParent(root);
		int iconCount = m_Manager->MaxSpells();
		m_SpellIcons.resize(iconCount);
		float padding = 10;
		m_IconWidth = (m_MenuWidth - (iconCount + 1) * padding) / (float)iconCount;
		for (int i = 0; i < iconCount; i++)
		{
			float x = (i - iconCount / 2) * (m_IconWidth + padding);
			float y = 0;
			GameObject* icon;
			if (m_Manager->HasSpell(i))
			{
				icon = m_Factory.Image(m_IconWidth, m_IconWidth, ResourceManager::Get<Texture2D>(GameManager::Get().Spells().GetSpell(m_Manager->GetSpellId(i)).SpellIconId), Transform({ x, 0, 1 }));
			}
			else
			{
				icon = m_Factory.Rectangle(m_IconWidth, m_IconWidth, Color::Red, Transform({ x, 0, 1 }));
			}
			GameObject* shade = m_Factory.Rectangle(m_IconWidth, m_IconWidth, Color(5, 5, 5, 200), Transform({ x, 0, 2 }));
			m_SpellIcons[i] = { icon, shade };
		}
		m_Factory.SetCurrentParent(nullptr);
	}

	void SpellCooldownMenu::Update()
	{
		for (int i = 0; i < m_Manager->MaxSpells(); i++)
		{
			float cd = m_Manager->CooldownOf(i);
			float rcd = m_Manager->RemainingCooldownOf(i);
			float prop = rcd / cd;
			m_SpellIcons[i].Shade->transform().SetLocalScale(1, std::clamp(prop, 0.00001f, 1.0f), 1);
			Vector3f localPos = m_SpellIcons[i].Shade->transform().LocalPosition();
			m_SpellIcons[i].Shade->transform().SetLocalPosition(localPos.x, (prop - 1) / 2.0f * m_IconWidth, localPos.z);
		}
	}

	void SpellCooldownMenu::DestroyMenu()
	{
		m_Manager = nullptr;
	}

}