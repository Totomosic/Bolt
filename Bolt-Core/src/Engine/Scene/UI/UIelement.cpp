#include "bltpch.h"
#include "UIElement.h"

#include "../Components/MeshRenderer.h"
#include "UIManager.h"
#include "../Layer.h"
#include "Graphics/Resources/ResourceManager.h"

#include "UICompoundElement.h"
#include "UISurface.h"
#include "Shapes/UIRectangle.h"
#include "UIText.h"
#include "Compounds/UITextInput.h"

namespace Bolt
{

	UIElement::UIElement(UIManager* manager, UIElement* parent)
		: m_Manager(manager), m_Parent(parent), m_Children(), m_GameObject(nullptr), m_Id(""), m_Events(), m_IsFocused(false), m_CompoundElement(nullptr)
	{
		SetGameObject(m_Manager->Factory().Instantiate());
		if (HasParent())
		{
			GetParent().Events().Bus().MountOn(m_Events.Bus());
		}

		m_Events.OnFocus.AddEventListener([this](Event<UIFocusEvent>& e)
			{
				// Don't propagate focus events if there is no compound element
				if (m_CompoundElement == nullptr)
				{
					e.StopPropagation();
				}
			}, ListenerPriority::Low);
		m_Events.OnFocusLost.AddEventListener([this](Event<UIFocusLostEvent>& e)
			{
				// Don't propagate focus events if there is no compound element
				if (m_CompoundElement == nullptr)
				{
					e.StopPropagation();
				}
			}, ListenerPriority::Low);
	}

	UIElement::~UIElement()
	{
		if (m_IsFocused)
		{
			Blur();
		}
		if (m_GameObject != nullptr)
		{
			Destroy(m_GameObject);
		}
	}

	UIElement& UIElement::GetParent() const
	{
		return *m_Parent;
	}

	bool UIElement::HasParent() const
	{
		return m_Parent != nullptr;
	}

	const std::vector<std::unique_ptr<UIElement>>& UIElement::GetChildren() const
	{
		return m_Children;
	}

	const blt::string& UIElement::GetId() const
	{
		return m_Id;
	}

	const Transform& UIElement::GetTransform() const
	{
		return m_GameObject->transform();
	}

	bool UIElement::HasMesh() const
	{
		return m_GameObject->Components().HasComponent<MeshRenderer>();
	}

	const Mesh& UIElement::GetMesh() const
	{
		return m_GameObject->mesh().Mesh;
	}

	Mesh& UIElement::GetMesh()
	{
		return m_GameObject->mesh().Mesh;
	}

	const UIEventHandler& UIElement::Events() const
	{
		return m_Events;
	}

	UIEventHandler& UIElement::Events()
	{
		return m_Events;
	}

	bool UIElement::HasFocus() const
	{
		return m_IsFocused;
	}

	GameObject* UIElement::GetGameObject() const
	{
		return m_GameObject;
	}

	UIManager* UIElement::GetManager() const
	{
		return m_Manager;
	}

	UIElement& UIElement::GetCompoundElement()
	{
		if (m_CompoundElement != nullptr)
		{
			return *m_CompoundElement;
		}
		return *this;
	}

	void UIElement::Focus()
	{
		m_IsFocused = true;
		m_Manager->SetFocusedElement(this);
		m_Events.OnFocus.Emit({ *this });
	}

	void UIElement::Blur()
	{
		m_IsFocused = false;
		m_Manager->SetFocusedElement(nullptr);
		m_Events.OnFocusLost.Emit({ *this });
	}

	void UIElement::Clear()
	{
		m_Children.clear();
	}

	void UIElement::SetId(const blt::string& id)
	{
		if (m_Id == "")
		{
			m_Manager->SetElementId(id, this);
		}
		else
		{
			m_Manager->UpdateElementId(m_Id, id);
		}
		m_Id = id;
	}

	void UIElement::SetCompoundElement(UIElement* element)
	{
		m_CompoundElement = element;
	}

	void UIElement::RemoveChildElement(UIElement& element)
	{
		auto it = std::find_if(m_Children.begin(), m_Children.end(), [&element](const std::unique_ptr<UIElement>& el)
			{
				return el.get() == &element;
			});
		if (it != m_Children.end())
		{
			m_Children.erase(it);
		}
	}

	void UIElement::Remove()
	{
		if (HasParent())
		{
			GetParent().RemoveChildElement(*this);
		}
	}

	bool UIElement::ContainsPoint(const Vector2f& point) const
	{
		if (!HasMesh())
		{
			return false;
		}
		Cuboid bounds = GetGameObject()->mesh().GetOverallMeshBounds();
		Vector3f min = GetTransform().TransformMatrix() * bounds.Min;
		Vector3f max = GetTransform().TransformMatrix() * bounds.Max;
		min.x = std::min(min.x, max.x);
		min.y = std::min(min.y, max.y);
		max.x = std::max(min.x, max.x);
		max.y = std::max(min.y, max.y);
		return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
	}

	UIElement& UIElement::CreateElement()
	{
		return AddChildElement(std::make_unique<UIElement>(m_Manager, this));
	}

	UICompoundElement& UIElement::CreateCompoundElement()
	{
		return (UICompoundElement&)AddChildElement(std::make_unique<UICompoundElement>(m_Manager, this));
	}

	UIRectangle& UIElement::CreateRectangle(float width, float height, std::unique_ptr<Material>&& material, Transform&& transform)
	{
		return (UIRectangle&)AddChildElement(std::make_unique<UIRectangle>(m_Manager, this, width, height, std::move(material), std::move(transform)));
	}

	UIRectangle& UIElement::CreateRectangle(float width, float height, const Color& color, Transform&& transform)
	{
		return CreateRectangle(width, height, ResourceManager::Get().Materials().Default(color), std::move(transform));
	}

	UIText& UIElement::CreateText(const blt::string& text, const ResourcePtr<Font>& font, const Color& color, Transform&& transform, AlignH horizontal, AlignV vertical)
	{
		return (UIText&)AddChildElement(std::make_unique<UIText>(m_Manager, this, text, font, color, std::move(transform), horizontal, vertical));
	}

	UIText& UIElement::CreateText(const blt::string& text, const Color& color, Transform&& transform, AlignH horizontal, AlignV vertical)
	{
		return CreateText(text, ResourceManager::Get().Fonts().Default(), color, std::move(transform), horizontal, vertical);
	}

	UITextInput& UIElement::CreateTextInput(float width, float height, const ResourcePtr<Font>& font, const Color& fontColor, Transform&& transform)
	{
		return CreateTextInput(width, height, Color::White, font, fontColor, std::move(transform));
	}

	UITextInput& UIElement::CreateTextInput(float width, float height, const Color& fontColor, Transform&& transform)
	{
		return CreateTextInput(width, height, ResourceManager::Get().Fonts().Default(), fontColor, std::move(transform));
	}

	UITextInput& UIElement::CreateTextInput(float width, float height, const Color& backgroundColor, const ResourcePtr<Font>& font, const Color& fontColor, Transform&& transform)
	{
		return CreateTextInput(width, height, ResourceManager::Get().Materials().Default(backgroundColor), font, fontColor, std::move(transform));
	}

	UITextInput& UIElement::CreateTextInput(float width, float height, const Color& backgroundColor, const Color& fontColor, Transform&& transform)
	{
		return CreateTextInput(width, height, backgroundColor, ResourceManager::Get().Fonts().Default(), fontColor, std::move(transform));
	}

	UITextInput& UIElement::CreateTextInput(float width, float height, std::unique_ptr<Material>&& material, const ResourcePtr<Font>& font, const Color& fontColor, Transform&& transform)
	{
		return (UITextInput&)AddChildElement(std::make_unique<UITextInput>(m_Manager, this, width, height, font, fontColor, std::move(material), std::move(transform)));
	}

	UITextInput& UIElement::CreateTextInput(float width, float height, std::unique_ptr<Material>&& material, const Color& fontColor, Transform&& transform)
	{
		return CreateTextInput(width, height, std::move(material), ResourceManager::Get().Fonts().Default(), fontColor, std::move(transform));
	}

	void UIElement::SetGameObject(GameObject* object)
	{
		if (m_GameObject)
		{
			Destroy(m_GameObject);
		}
		m_GameObject = object;
		if (HasParent())
		{
			m_GameObject->MakeChildOf(GetParent().GetGameObject());
		}
	}

	UIElement& UIElement::AddChildElement(std::unique_ptr<UIElement>&& element)
	{
		UIElement* ptr = element.get();
		m_Children.push_back(std::move(element));
		return *ptr;
	}

}