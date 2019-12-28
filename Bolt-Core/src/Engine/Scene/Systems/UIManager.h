#pragma once
#include "System.h"
#include "UI/UIElement.h"

namespace Bolt
{

	class BLT_API UIManager : public System<UIManager>
	{
	private:
		EntityManager* m_Manager;
		EntityFactory m_Factory;
		std::unique_ptr<UIElement> m_Root;
		
		mutable UIElement* m_FocusedElement;

		ScopedEventListener m_ClickListener;
		ScopedEventListener m_MouseDownListener;
		ScopedEventListener m_MouseUpListener;
		ScopedEventListener m_KeyDownListener;
		ScopedEventListener m_KeyUpListener;
		ScopedEventListener m_CharPressedListener;

	public:
		UIManager(EntityManager* manager);

		const EntityFactory& GetFactory() const;

		virtual void Update(EntityManager& manager, TimeDelta delta) override;
	};

}