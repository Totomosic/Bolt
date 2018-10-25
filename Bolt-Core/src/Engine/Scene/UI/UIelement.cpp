#include "UIelement.h"
#include "..\Layer.h"

namespace Bolt
{

	UIelement::UIelement() : UIelement(nullptr)
	{
	
	}

	UIelement::UIelement(GameObject* object)
		: m_ParentElement(nullptr), m_GameObject(object), m_Elements()
	{
	
	}

	UIelement::~UIelement()
	{
		if (m_GameObject != nullptr)
		{
			Destroy(m_GameObject, 0.0f);
		}
	}

	GameObject* UIelement::Object() const
	{
		return m_GameObject;
	}

	UIelement* UIelement::Parent() const
	{
		return m_ParentElement;
	}

	UIelement* UIelement::GetElementById(id_t id) const
	{
		return m_Elements.at(id).get();
	}

	int UIelement::ElementCount() const
	{
		return m_Elements.size();
	}

	void UIelement::SetParent(UIelement* parent)
	{
		UIelement* oldParent = m_ParentElement;
		m_ParentElement = parent;
		if (m_GameObject != nullptr)
		{
			m_GameObject->MakeChildOf(m_ParentElement->Object());
		}
		if (oldParent == nullptr)
		{
			CreateGameObject();
		}
	}

	id_t UIelement::AddElement(std::unique_ptr<UIelement>&& element)
	{
		id_t id = m_Elements.size();
		element->SetParent(this);
		m_Elements.push_back(std::move(element));
		return id;
	}

	void UIelement::Clear()
	{
		m_Elements.clear();
	}

	void UIelement::SetGameObject(GameObject* object)
	{
		m_GameObject = object;
		if (m_ParentElement != nullptr)
		{
			m_GameObject->MakeChildOf(m_ParentElement->Object());
		}
	}

	void UIelement::CreateGameObject()
	{
		m_GameObject = nullptr;
	}

}