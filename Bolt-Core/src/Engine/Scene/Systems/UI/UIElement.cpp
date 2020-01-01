#include "bltpch.h"
#include "UIElement.h"

#include "../UIManager.h"

namespace Bolt
{

	UIElement::UIElement(UIManager* manager, UIElement* parent) : UIElement(manager, parent, EntityHandle())
	{
		
	}

	UIElement::UIElement(UIManager* manager, UIElement* parent, const EntityHandle& entity) : UIFactory(manager, *this),
		m_Manager(manager), m_Entity(), m_IsFocused(false), m_Parent(nullptr), m_Children(), m_CompoundElement(nullptr)
	{
		SetParent(parent);
		SetEntity(entity);
	}

	UIElement::~UIElement()
	{
		if (HasFocus())
		{
			Blur();
		}
		m_Children.clear();
		if (m_Entity.IsValid())
		{
			m_Entity.Destroy();
		}
	}

	bool UIElement::HasParent() const
	{
		return m_Parent != nullptr;
	}

	UIElement& UIElement::GetParent() const
	{
		return *m_Parent;
	}

	const std::vector<std::unique_ptr<UIElement>>& UIElement::GetChildren() const
	{
		return m_Children;
	}

	EntityHandle UIElement::GetEntity() const
	{
		return m_Entity;
	}

	const UIEvents& UIElement::Events() const
	{
		BLT_ASSERT(m_Entity.IsValid(), "Entity is not valid");
		return *m_Entity.GetComponent<UIEvents>();
	}

	UIEvents& UIElement::Events()
	{
		BLT_ASSERT(m_Entity.IsValid(), "Entity is not valid");
		return *m_Entity.GetComponent<UIEvents>();
	}

	const Transform& UIElement::GetTransform() const
	{
		BLT_ASSERT(m_Entity.IsValid(), "Entity is not valid");
		return *m_Entity.GetComponent<Transform>();
	}

	Transform& UIElement::GetTransform()
	{
		BLT_ASSERT(m_Entity.IsValid(), "Entity is not valid");
		return *m_Entity.GetComponent<Transform>();
	}

	bool UIElement::HasMesh() const
	{
		BLT_ASSERT(m_Entity.IsValid(), "Entity is not valid");
		return m_Entity.HasComponent<Mesh>();
	}

	const Mesh& UIElement::GetMesh() const
	{
		BLT_ASSERT(m_Entity.IsValid(), "Entity is not valid");
		return *m_Entity.GetComponent<Mesh>();
	}

	Mesh& UIElement::GetMesh()
	{
		BLT_ASSERT(m_Entity.IsValid(), "Entity is not valid");
		return *m_Entity.GetComponent<Mesh>();
	}

	bool UIElement::HasFocus() const
	{
		return m_IsFocused;
	}

	UIElement& UIElement::AddChild(std::unique_ptr<UIElement>&& element)
	{
		UIElement& child = *element;
		m_Children.push_back(std::move(element));
		child.SetParent(this);
		return child;
	}

	void UIElement::RemoveChild(UIElement& element)
	{
		auto it = std::find_if(m_Children.begin(), m_Children.end(), [&element](const std::unique_ptr<UIElement>& e)
			{
				return &element == e.get();
			});
		if (it != m_Children.end())
		{
			m_Children.erase(it);
		}
	}

	void UIElement::ClearChildren()
	{
		m_Children.clear();
	}

	bool UIElement::HasCompoundElement() const
	{
		return m_CompoundElement != nullptr;
	}

	UIElement& UIElement::GetCompoundElement() const
	{
		return *m_CompoundElement;
	}

	void UIElement::SetCompoundElement(UIElement* element)
	{
		m_CompoundElement = element;
	}

	void UIElement::Remove()
	{
		if (HasParent())
		{
			GetParent().RemoveChild(*this);
		}
	}

	void UIElement::Focus()
	{
		m_Manager->SetFocusedElement(this);
	}

	void UIElement::Blur()
	{
		if (m_IsFocused)
		{
			m_Manager->SetFocusedElement(nullptr);
		}
	}

	bool UIElement::ContainsPoint(const Vector2f& point) const
	{
		if (!HasMesh())
		{
			return false;
		}
		const Mesh& mesh = GetMesh();
		const Transform& transform = GetTransform();
		Cuboid bounds = mesh.ComputeBoundingBox(transform.TransformMatrix());
		return point.x >= bounds.Min.x && point.x <= bounds.Max.x && point.y >= bounds.Min.y && point.y <= bounds.Max.y;
	}

	const EntityFactory& UIElement::GetEntityFactory() const
	{
		return m_Manager->GetFactory();
	}

	EntityHandle UIElement::SetEntity(EntityHandle entity)
	{
		if (entity == m_Entity)
			return m_Entity;
		BLT_ASSERT(ValidateEntity(entity), "Invalid Entity");
		if (m_Entity.IsValid())
		{
			m_Entity.Destroy();
		}
		m_Entity = entity;
		if (!m_Entity.HasComponent<UIEvents>())
		{
			m_Entity.Assign<UIEvents>();
		}
		UIEvents& events = *m_Entity.GetComponent<UIEvents>();
		// Update is focused
		events.OnFocus().AddEventListener([this](Event<UI<UIFocus>>& e)
			{
				m_IsFocused = true;
			});
		events.OnFocusLost().AddEventListener([this](Event<UI<UIFocusLost>>& e)
			{
				m_IsFocused = false;
			});
		// Stop focus events propagating if no compound element
		events.OnFocus().AddEventListener([this](Event<UI<UIFocus>>& e)
			{
				if (!HasCompoundElement())
				{
					e.StopPropagation();
				}
			}, ListenerPriority::Low);
		events.OnFocusLost().AddEventListener([this](Event<UI<UIFocusLost>>& e)
			{
				if (!HasCompoundElement())
				{
					e.StopPropagation();
				}
			}, ListenerPriority::Low);
		SetupEntity(m_Entity);
		return m_Entity;
	}

	bool UIElement::ValidateEntity(EntityHandle entity) const
	{
		return entity.HasComponent<Transform>();
	}

	void UIElement::SetParent(UIElement* parent)
	{
		if (parent == m_Parent)
			return;
		BLT_ASSERT(m_Parent == nullptr, "Cannot change UI element's parent");
		m_Parent = parent;
		if (m_Entity.IsValid())
		{
			SetupEntity(m_Entity);
		}
	}

	void UIElement::SetupEntity(EntityHandle entity) const
	{
		Transform& transform = *entity.GetComponent<Transform>();
		if (!HasParent())
		{
			transform.SetParent(nullptr);
		}
		else if (GetParent().GetEntity().IsValid())
		{
			Transform& parentTransform = GetParent().GetTransform();
			transform.SetParent(&parentTransform);
			UIEvents& events = *m_Entity.GetComponent<UIEvents>();
			UIEvents& parentEvents = GetParent().Events();
			parentEvents.Bus().MountOn(events.Bus());
		}
	}

}