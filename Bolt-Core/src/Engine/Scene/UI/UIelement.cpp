#include "UIelement.h"
#include "..\Layer.h"

#include "UIsurface.h"
#include "Text.h"

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

	id_t UIelement::Id() const
	{
		return m_Id;
	}

	GameObject* UIelement::Object() const
	{
		return m_GameObject;
	}

	UIEventHandler& UIelement::EventHandler() const
	{
		return Object()->Components().GetComponent<UIEventHandler>();
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

	UIelement* UIelement::AddElement(std::unique_ptr<UIelement>&& element)
	{
		id_t id = m_Elements.size();
		UIelement* ptr = element.get();
		ptr->SetId(id);
		element->SetParent(this);
		m_Elements.push_back(std::move(element));
		return ptr;
	}

	void UIelement::Clear()
	{
		m_Elements.clear();
	}

	Text* UIelement::Text(const blt::string& text, const ResourcePtr<const Font>& font, const Color& color, Transform&& transform, AlignH horizontal, AlignV vertical)
	{
		return (Bolt::Text*)AddElement<Bolt::Text>(text, font, color, std::move(transform), horizontal, vertical);
	}

	Text* UIelement::Text(const blt::string& text, const Color& color, Transform&& transform, AlignH horizontal, AlignV vertical)
	{
		BLT_ASSERT(ResourceManager::DefaultFont() != nullptr, "No default font");
		return Text(text, ResourceManager::DefaultFont(), color, std::move(transform), horizontal, vertical);
	}

	UIsurface* UIelement::Rectangle(float width, float height, const Color& color, Transform&& transform)
	{
		return (UIsurface*)AddElement<UIsurface>(Vector2f(width, height), color, std::move(transform));
	}

	UIsurface* UIelement::Rectangle(float width, float height, const Material& material, Transform&& transform)
	{
		return (UIsurface*)AddElement<UIsurface>(Vector2f(width, height), material, std::move(transform));
	}

	UIsurface* UIelement::Image(float width, float height, const ResourcePtr<const Texture2D>& texture, Transform&& transform)
	{
		return (UIsurface*)AddElement<UIsurface>(Vector2f(width, height), texture, std::move(transform));
	}

	void UIelement::ReleaseAllGameObjects()
	{
		for (auto& ptr : m_Elements)
		{
			ptr.release();
		}
	}

	void UIelement::SetId(id_t id)
	{
		m_Id = id;
	}

	void UIelement::SetGameObject(GameObject* object)
	{
		m_GameObject = object;
		if (!m_GameObject->Components().HasComponent<UIEventHandler>())
		{
			m_GameObject->Components().AddComponent<UIEventHandler>();
		}
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