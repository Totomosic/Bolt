#include "bltpch.h"
#include "UIManager.h"

#include "Engine/User/Input.h"
#include "Engine/Scene/SceneManager.h"

namespace Bolt
{

	UIManager::UIManager(Layer* layer)
		: m_Factory(*layer), m_RootElement(), m_FocusedElement(nullptr), m_ElementIdMap(), m_TabElements(), m_TabIndex(-1),
		m_MouseClickedHandler(), m_MouseDownHandler(), m_MouseUpHandler(), m_KeyDownHandler(), m_KeyUpHandler(), m_CharPressedHandler()
	{
		m_MouseClickedHandler = Input::Get().OnMouseClicked.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::MouseClickHandler), ListenerPriority::High);
		m_MouseDownHandler = Input::Get().OnMousePressed.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::MouseDownHandler), ListenerPriority::High);
		m_MouseUpHandler = Input::Get().OnMouseReleased.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::MouseUpHandler), ListenerPriority::High);
		m_KeyDownHandler = Input::Get().OnKeyPressed.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::KeyDownHandler), ListenerPriority::High);
		m_KeyUpHandler = Input::Get().OnKeyReleased.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::KeyUpHandler), ListenerPriority::High);
		m_CharPressedHandler = Input::Get().OnCharPressed.AddScopedEventListener(BLT_BIND_EVENT_FN(UIManager::CharPressedHandler), ListenerPriority::High);
	}

	void UIManager::Initialize()
	{
		m_RootElement = std::make_unique<UIElement>(this, nullptr);
	}

	const ObjectFactory& UIManager::Factory() const
	{
		return m_Factory;
	}

	ObjectFactory& UIManager::Factory()
	{
		return m_Factory;
	}

	const UIElement& UIManager::Root() const
	{
		return *m_RootElement;
	}

	UIElement& UIManager::Root()
	{
		return *m_RootElement;
	}

	bool UIManager::IsActive() const
	{
		return m_Factory.CurrentLayer()->IsActive();
	}

	UIElement& UIManager::GetElementById(const blt::string& id) const
	{
		BLT_ASSERT(m_ElementIdMap.find(id) != m_ElementIdMap.end(), "No element with id {} exists", id);
		return *m_ElementIdMap.at(id);
	}

	void UIManager::AddElementToTabList(UIElement* element)
	{
		m_TabElements.push_back(element);
	}

	void UIManager::InsertElementIntoTabList(int index, UIElement* element)
	{
		index = Clamp(index, 0, (int)m_TabElements.size());
		if (index == m_TabElements.size())
		{
			AddElementToTabList(element);
		}
		else
		{
			m_TabElements.insert(m_TabElements.begin() + index, element);
		}
	}

	void UIManager::SetCurrentTabIndex(int index)
	{
		m_TabIndex = index;
		if (m_TabIndex == -1)
		{
			SetFocusedElementInternal(nullptr);
		}
		else
		{
			m_TabIndex = m_TabIndex % m_TabElements.size();
			if (m_TabIndex < m_TabElements.size())
			{
				SetFocusedElementInternal(m_TabElements[m_TabIndex]);
			}
		}
	}

	void UIManager::Clear() const
	{
		m_RootElement->Clear();
		m_FocusedElement = nullptr;
	}

	void UIManager::OnActivate()
	{
		m_TabIndex = -1;
		m_FocusedElement = nullptr;
	}

	void UIManager::OnDeactivate()
	{
		m_TabIndex = -1;
		if (m_FocusedElement != nullptr)
		{
			m_FocusedElement->Blur();
			m_FocusedElement = nullptr;
		}
	}

	void UIManager::SetFocusedElement(UIElement* element)
	{
		if (m_FocusedElement != nullptr && element != nullptr)
		{
			m_FocusedElement->Blur();
		}
		m_FocusedElement = element;
	}

	void UIManager::SetFocusedElementInternal(UIElement* element)
	{
		if (element != nullptr)
		{
			element->Focus();
		}
		else if (m_FocusedElement != nullptr)
		{
			m_FocusedElement->Blur();
		}
		m_FocusedElement = element;
	}

	std::vector<UIElement*> UIManager::GetElementsUnderPoint(const Vector2f& point, int limit) const
	{
		std::vector<UIElement*> result;
		if (m_RootElement != nullptr)
		{
			GetElementsUnderPointRecursive(point, m_RootElement.get(), result, limit);
			std::sort(result.begin(), result.end(), [](UIElement* left, UIElement* right)
				{
					return right->GetTransform().Position().z < left->GetTransform().Position().z;
				});
		}
		return result;
	}

	void UIManager::GetElementsUnderPointRecursive(const Vector2f& point, UIElement* currentElement, std::vector<UIElement*>& elements, int limit) const
	{
		if (elements.size() < limit || limit == -1)
		{
			if (currentElement->ContainsPoint(point))
			{
				elements.push_back(currentElement);
			}
			if (!(elements.size() < limit || limit == -1))
			{
				return;
			}
			for (const std::unique_ptr<UIElement>& element : currentElement->GetChildren())
			{
				GetElementsUnderPointRecursive(point, element.get(), elements, limit);
				if (!(elements.size() < limit || limit == -1))
				{
					break;
				}
			}
		}
	}

	void UIManager::SetElementId(const blt::string& id, UIElement* element)
	{
		BLT_ASSERT(m_ElementIdMap.find(id) == m_ElementIdMap.end(), "Element with id {} already exists", id);
		m_ElementIdMap[id] = element;
	}

	void UIManager::UpdateElementId(const blt::string& oldId, const blt::string& newId)
	{
		BLT_ASSERT(m_ElementIdMap.find(oldId) != m_ElementIdMap.end(), "No element with id {} exists", oldId);
		UIElement* element = m_ElementIdMap.at(oldId);
		m_ElementIdMap.erase(oldId);
		SetElementId(newId, element);
	}

	// =============================================================================================================================================================================
	// EVENT CALLBACKS
	// =============================================================================================================================================================================

	void UIManager::MouseClickHandler(Event<MouseClickEvent>& e)
	{		
		if (IsActive())
		{
			Vector2f point = RemapMousePosition({ e.Data.x, e.Data.y });
			if (m_FocusedElement != nullptr)
			{
				m_FocusedElement->Events().OnClick.Emit({ *m_FocusedElement, point, e.Data.Button });
			}
		}
	}

	void UIManager::MouseDownHandler(Event<MousePressedEvent>& e)
	{
		if (IsActive())
		{
			Vector2f point = RemapMousePosition({ e.Data.x, e.Data.y });
			std::vector<UIElement*> elements = GetElementsUnderPoint(point);
			if (elements.size() > 0)
			{
				UIElement* selectedElement = elements[0];
				if (m_FocusedElement != selectedElement)
				{
					selectedElement->Focus();
					auto it = std::find(m_TabElements.begin(), m_TabElements.end(), &selectedElement->GetCompoundElement());
					if (it != m_TabElements.end())
					{
						auto index = it - m_TabElements.begin();
						m_TabIndex = (int)index;
					}
				}
				selectedElement->Events().OnMouseDown.Emit({ *selectedElement, point, e.Data.Button });
				e.StopPropagation();
			}
			else if (m_FocusedElement != nullptr)
			{
				SetFocusedElementInternal(nullptr);
				m_TabIndex = -1;
			}
		}
	}

	void UIManager::MouseUpHandler(Event<MouseReleasedEvent>& e)
	{
		if (IsActive())
		{
			if (m_FocusedElement != nullptr)
			{
				Vector2f point = RemapMousePosition({ e.Data.x, e.Data.y });
				m_FocusedElement->Events().OnMouseUp.Emit({ *m_FocusedElement, point, e.Data.Button });
			}
		}
	}

	void UIManager::KeyDownHandler(Event<KeyPressedEvent>& e)
	{
		if (IsActive())
		{
			if (m_TabElements.size() > 0 && e.Data.KeyCode == Keycode::Tab && m_FocusedElement != nullptr)
			{
				SetCurrentTabIndex(m_TabIndex + 1);
			}
			if (m_FocusedElement != nullptr)
			{
				m_FocusedElement->Events().OnKeyDown.Emit({ *m_FocusedElement, e.Data.KeyCode, e.Data.IsRepeat });
			}
		}
	}

	void UIManager::KeyUpHandler(Event<KeyReleasedEvent>& e)
	{
		if (IsActive())
		{
			if (m_FocusedElement != nullptr)
			{
				m_FocusedElement->Events().OnKeyUp.Emit({ *m_FocusedElement, e.Data.KeyCode });
			}
		}
	}

	void UIManager::CharPressedHandler(Event<CharPressedEvent>& e)
	{
		if (IsActive())
		{
			if (m_FocusedElement != nullptr)
			{
				m_FocusedElement->Events().OnCharPressed.Emit({ *m_FocusedElement, e.Data.CharCode });
			}
		}
	}

	Vector2f UIManager::RemapMousePosition(const Vector2f& position) const
	{
		Camera* camera = m_Factory.CurrentLayer()->ActiveCamera();
		if (camera)
		{
			float width = camera->ViewWidth();
			float height = camera->ViewHeight();
			return Input::Get().MousePosition(width, height).xy();
		}
		return position;
	}

}