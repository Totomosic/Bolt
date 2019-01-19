#pragma once
#include "UImenu.h"
#include "../Entities/SpellManager.h"

namespace DND
{

	class SpellCooldownMenu : public UImenu
	{
	public:
		struct SpellIcon
		{
			GameObject* Icon;
			GameObject* Shade;
		};

	private:
		ObjectFactory m_Factory;
		SpellManager* m_Manager;
		std::vector<SpellIcon> m_SpellIcons;

		Vector3f m_MenuPosition;
		float m_MenuWidth;
		float m_MenuHeight;
		float m_IconWidth;

	public:
		SpellCooldownMenu(Layer* layer, const Vector3f& position, float width, float height, SpellManager* manager);

		void CreateMenu() override;
		void Update() override;
		void DestroyMenu() override;

	};

}