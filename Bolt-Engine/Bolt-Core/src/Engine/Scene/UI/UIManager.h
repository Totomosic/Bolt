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
		
		ScopedEventListener m_MouseClickedHandler;

	public:
		UIManager(Layer* layer);

		void Initialize();

		const ObjectFactory& Factory() const;
		ObjectFactory& Factory();
		const UIElement& Root() const;
		UIElement& Root();

		void Clear() const;

		friend class UIElement;

	private:
		void SetFocusedElement(UIElement* element);
		std::vector<UIElement*> GetElementsUnderPoint(const Vector2f& point, int limit = -1) const;
		void GetElementsUnderPointRecursive(const Vector2f& point, UIElement* currentElement, std::vector<UIElement*>& elements, int limit) const;

	};

}