#pragma once
#include "UIElement.h"
#include "../ObjectFactory.h"

namespace Bolt
{

	class BLT_API UIManager
	{
	private:
		ObjectFactory m_Factory;
		std::unique_ptr<UIElement> m_RootElement;
		UIElement* m_FocusedElement;
		std::unordered_map<blt::string, UIElement*> m_ElementIdMap;
		
		ScopedEventListener m_MouseClickedHandler;
		ScopedEventListener m_MouseDownHandler;
		ScopedEventListener m_MouseUpHandler;
		ScopedEventListener m_KeyDownHandler;
		ScopedEventListener m_KeyUpHandler;
		ScopedEventListener m_CharPressedHandler;

	public:
		UIManager(Layer* layer);

		void Initialize();

		const ObjectFactory& Factory() const;
		ObjectFactory& Factory();
		const UIElement& Root() const;
		UIElement& Root();
		bool IsActive() const;
		UIElement& GetElementById(const blt::string& id) const;

		template<typename T>
		T& GetElementById(const blt::string& id) const
		{
			return (T&)GetElementById(id);
		}

		void Clear() const;

		friend class UIElement;

	private:
		void SetFocusedElement(UIElement* element);
		std::vector<UIElement*> GetElementsUnderPoint(const Vector2f& point, int limit = -1) const;
		void GetElementsUnderPointRecursive(const Vector2f& point, UIElement* currentElement, std::vector<UIElement*>& elements, int limit) const;

		void SetElementId(const blt::string& id, UIElement* element);
		void UpdateElementId(const blt::string& oldId, const blt::string& newId);

		void MouseClickHandler(Event<MouseClickEvent>& e);
		void MouseDownHandler(Event<MousePressedEvent>& e);
		void MouseUpHandler(Event<MouseReleasedEvent>& e);
		void KeyDownHandler(Event<KeyPressedEvent>& e);
		void KeyUpHandler(Event<KeyReleasedEvent>& e);
		void CharPressedHandler(Event<CharPressedEvent>& e);

	};

}