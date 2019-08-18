#include "bltpch.h"
#include "UIManager.h"

#include "Engine/User/Input.h"
#include "Engine/Scene/SceneManager.h"

namespace Bolt
{

	UIManager::UIManager(Layer* layer)
		: m_Factory(*layer), m_RootElement(), m_FocusedElement(nullptr),
		m_MouseClickListener(0)
	{
		m_MouseClickListener = Input::Get().OnMouseClicked.AddEventListener([this](Event<MouseClickEvent>& e)
			{
				if (m_Factory.CurrentLayer()->IsActive())
				{
					Vector2f point(e.Data.x, e.Data.y);
					std::vector<UIElement*> elements = GetElementsUnderPoint(point);
					if (elements.size() > 0)
					{
						UIElement* selectedElement = elements[0];
						if (m_FocusedElement != selectedElement)
						{
							selectedElement->Focus();
						}
						selectedElement->Events().OnClick.Emit({ *selectedElement, { e.Data.x, e.Data.y }, e.Data.Button });
						e.StopPropagation();
					}
					else if (m_FocusedElement != nullptr)
					{
						m_FocusedElement->Blur();
					}
				}
			}, ListenerPriority::High);
	}

	UIManager::~UIManager()
	{
		Input::Get().OnMouseClicked.RemoveEventListener(m_MouseClickListener);
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

	void UIManager::Clear() const
	{
		m_RootElement->Clear();
	}

	void UIManager::SetFocusedElement(UIElement* element)
	{
		if (m_FocusedElement != nullptr && element != nullptr)
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
					return right->GetTransform().Position().z <= left->GetTransform().Position().z;
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

}