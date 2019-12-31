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
		mutable UIElement* m_FocusedElement;
		std::unique_ptr<UIElement> m_Root;	

		ScopedEventListener m_ClickListener;
		ScopedEventListener m_MouseDownListener;
		ScopedEventListener m_MouseUpListener;
		ScopedEventListener m_KeyDownListener;
		ScopedEventListener m_KeyUpListener;
		ScopedEventListener m_CharPressedListener;

	public:
		UIManager(EntityManager* manager);

		const EntityFactory& GetFactory() const;
		UIElement& GetRoot() const;

		bool HasFocusedElement() const;
		UIElement& GetFocusedElement() const;
		void SetFocusedElement(UIElement* element) const;

		virtual void Update(EntityManager& manager, TimeDelta delta) override;

		friend class UIElement;

	private:
		Vector2f ReprojectPoint(const Vector2f& point) const;
		Vector3f GetRelPosition(const Vector3f& point, const Transform& transform) const;
		std::vector<UIElement*> GetElementsUnderPoint(const Vector2f& point, int limit = -1) const;
		void GetElementsUnderPointRecursive(std::vector<UIElement*>& elements, UIElement& currentElement, const Vector2f& point, int& limit) const;

		void OnClickHandler(Event<MouseClickEvent>& e);
		void OnMouseDownHandler(Event<MouseDownEvent>& e);
		void OnMouseUpHandler(Event<MouseUpEvent>& e);
		void OnKeyDownHandler(Event<KeyDownEvent>& e);
		void OnKeyUpHandler(Event<KeyUpEvent>& e);
		void OnCharHandler(Event<CharPressedEvent>& e);

		template<typename T>
		T ReprojectEventData(const T& e) const
		{
			T result = e;
			Vector2f proj = ReprojectPoint({ e.x, e.y });
			result.x = proj.x;
			result.y = proj.y;
			return result;
		}
	};

}