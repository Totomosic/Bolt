#include "bltpch.h"
#include "UIManager.h"

namespace Bolt
{

	UIManager::UIManager(EntityManager* manager)
		: m_Manager(manager), m_Factory(*m_Manager), m_Root(nullptr), m_FocusedElement(nullptr),
		m_ClickListener(), m_MouseDownListener(), m_MouseUpListener(), m_KeyDownListener(), m_KeyUpListener(), m_CharPressedListener()
	{
		m_Root = std::make_unique<UIElement>(this, nullptr, GetFactory().CreateTransform());

		m_ClickListener = Input::Get().OnMouseClicked.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::OnClickHandler));
		m_MouseDownListener = Input::Get().OnMouseDown.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::OnMouseDownHandler));
		m_MouseUpListener = Input::Get().OnMouseUp.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::OnMouseUpHandler));
		m_KeyDownListener = Input::Get().OnKeyDown.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::OnKeyDownHandler));
		m_KeyUpListener = Input::Get().OnKeyUp.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::OnKeyUpHandler));
		m_CharPressedListener = Input::Get().OnCharPressed.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::OnCharHandler));
	}

	const EntityFactory& UIManager::GetFactory() const
	{
		return m_Factory;
	}

	UIElement& UIManager::GetRoot() const
	{
		return *m_Root;
	}

	bool UIManager::HasFocusedElement() const
	{
		return m_FocusedElement != nullptr;
	}

	UIElement& UIManager::GetFocusedElement() const
	{
		return *m_FocusedElement;
	}

	void UIManager::SetFocusedElement(UIElement* element) const
	{
		if (element == m_FocusedElement)
			return;
		if (m_FocusedElement != nullptr)
		{
			UIFocusLost e;
			EmitEvent(GetFocusedElement(), GetFocusedElement().Events().OnFocusLost(), e);
		}
		m_FocusedElement = element;
		if (m_FocusedElement != nullptr)
		{
			UIFocus e;
			EmitEvent(GetFocusedElement(), GetFocusedElement().Events().OnFocus(), e);
		}
	}

	void UIManager::Update(EntityManager& manager, TimeDelta delta)
	{
		if (IsEnabled())
		{

		}
	}

	Vector2f UIManager::ReprojectPoint(const Vector2f& point) const
	{
		return point;
	}

	Vector3f UIManager::GetRelPosition(const Vector3f& point, const Transform& transform) const
	{
		return point - transform.Position();
	}

	std::vector<UIElement*> UIManager::GetElementsUnderPoint(const Vector2f& point, int limit) const
	{
		std::vector<UIElement*> result;
		GetElementsUnderPointRecursive(result, GetRoot(), point, limit);
		// Sort?
		return result;
	}

	void UIManager::GetElementsUnderPointRecursive(std::vector<UIElement*>& elements, UIElement& currentElement, const Vector2f& point, int& limit) const
	{
		if (limit == 0)
			return;
		for (const std::unique_ptr<UIElement>& child : currentElement.GetChildren())
		{
			GetElementsUnderPointRecursive(elements, *child, point, limit);
			if (limit == 0)
				return;
		}
		if (currentElement.ContainsPoint(point))
		{
			elements.push_back(&currentElement);
			limit -= 1;
			if (limit == 0)
				return;
		}
	}

	void UIManager::OnClickHandler(Event<MouseClickEvent>& e)
	{
		if (IsEnabled())
		{
			if (HasFocusedElement())
			{
				EmitEvent(GetFocusedElement(), GetFocusedElement().Events().OnClick(), ReprojectEventData(e.Data));
			}
		}
	}

	void UIManager::OnMouseDownHandler(Event<MouseDownEvent>& e)
	{
		if (IsEnabled())
		{
			Vector2f mousePoint = ReprojectPoint({ e.Data.x, e.Data.y });
			std::vector<UIElement*> elements = GetElementsUnderPoint(mousePoint, 1);
			if (!elements.empty())
			{
				UIElement* element = elements.front();
				SetFocusedElement(element);
			}
			else
			{
				SetFocusedElement(nullptr);
			}

			if (HasFocusedElement())
			{
				EmitEvent(GetFocusedElement(), GetFocusedElement().Events().OnMouseDown(), ReprojectEventData(e.Data));
			}
		}
	}

	void UIManager::OnMouseUpHandler(Event<MouseUpEvent>& e)
	{
		if (IsEnabled())
		{
			if (HasFocusedElement())
			{
				EmitEvent(GetFocusedElement(), GetFocusedElement().Events().OnMouseUp(), ReprojectEventData(e.Data));
			}
		}
	}

	void UIManager::OnKeyDownHandler(Event<KeyDownEvent>& e)
	{
		if (IsEnabled())
		{
			if (HasFocusedElement())
			{
				EmitEvent(GetFocusedElement(), GetFocusedElement().Events().OnKeyDown(), e.Data);
			}
		}
	}

	void UIManager::OnKeyUpHandler(Event<KeyUpEvent>& e)
	{
		if (IsEnabled())
		{
			if (HasFocusedElement())
			{
				EmitEvent(GetFocusedElement(), GetFocusedElement().Events().OnKeyUp(), e.Data);
			}
		}
	}

	void UIManager::OnCharHandler(Event<CharPressedEvent>& e)
	{
		if (IsEnabled())
		{
			if (HasFocusedElement())
			{
				EmitEvent(GetFocusedElement(), GetFocusedElement().Events().OnCharPressed(), e.Data);
			}
		}
	}

}