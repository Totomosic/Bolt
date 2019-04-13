#include "Types.h"

#include "UIelement.h"
#include "UIroot.h"
#include "..\Layer.h"

#include "UIsurface.h"
#include "Text.h"

namespace Bolt
{

	UIelement::UIelement() : UIelement(nullptr)
	{
		
	}

	UIelement::UIelement(GameObject* object)
		: m_Root(nullptr), m_Parent(nullptr), m_Children(), m_Object(object)
	{

	}

	UIelement::~UIelement()
	{
		if (m_Object != nullptr)
		{
			Destroy(m_Object);
		}
	}

	GameObject* UIelement::Object() const
	{
		return m_Object;
	}

	UIelement& UIelement::Parent() const
	{
		return *m_Parent;
	}

	UIEventHandler& UIelement::EventHandler() const
	{
		return m_Object->Components().GetComponent<UIEventHandler>();
	}

	UIelement& UIelement::GetElement(id_t index) const
	{
		return *m_Children[index].get();
	}

	UIelement& UIelement::AddElement(std::unique_ptr<UIelement>&& element)
	{
		UIelement* ptr = element.get();
		m_Children.push_back(std::move(element));
		ptr->SetUIroot(m_Root);
		ptr->SetParent(this);
		return *ptr;
	}

	void UIelement::RemoveElement(UIelement* element)
	{
		auto it = std::find_if(m_Children.begin(), m_Children.end(), [element](const std::unique_ptr<UIelement>& item)
		{
			return element == item.get();
		});
		if (it != m_Children.end())
		{
			m_Children.erase(it);
		}
		else
		{
			BLT_CORE_ERROR("Unable to find UIelement");
		}
	}

	void UIelement::ReleaseGameObjects()
	{
		for (std::unique_ptr<UIelement>& child : m_Children)
		{
			child->ReleaseGameObjects();
			child->m_Object = nullptr;
		}
	}

	void UIelement::Clear()
	{
		m_Children.clear();
	}

	UIsurface& UIelement::Rectangle(float width, float height, const Color& color, Transform&& transform)
	{
		return AddElement<UIsurface>(width, height, color, std::move(transform));
	}

	UIsurface& UIelement::Rectangle(float width, float height, std::unique_ptr<Material>&& material, Transform&& transform)
	{
		return AddElement<UIsurface>(width, height, std::move(material), std::move(transform));
	}

	UIsurface& UIelement::Image(float width, float height, const ResourcePtr<const Texture2D>& texture, Transform&& transform)
	{
		std::unique_ptr<Material> material = ResourceManager::Get().Materials().Texture(texture);
		return AddElement<UIsurface>(width, height, std::move(material), std::move(transform));
	}

	Bolt::Text& UIelement::Text(const blt::string& text, const ResourcePtr<const Font>& font, const Color& color, Transform&& transform, AlignH horizontal, AlignV vertical)
	{
		return AddElement<Bolt::Text>(text, font, color, std::move(transform), horizontal, vertical);
	}

	Bolt::Text& UIelement::Text(const blt::string& text, const Color& color, Transform&& transform, AlignH horizontal, AlignV vertical)
	{
		return AddElement<Bolt::Text>(text, ResourceManager::Get().DefaultFont(), color, std::move(transform), horizontal, vertical);
	}

	void UIelement::SetUIroot(UIroot* root)
	{
		m_Root = root;
		if (m_Object == nullptr)
		{
			m_Object = m_Root->GetFactory().Instantiate();
			m_Object->Components().AddComponent<UIEventHandler>();
		}
		BLT_ASSERT(m_Object->GetLayer() == m_Root->GetFactory().CurrentLayer(), "Layer Mismatch");
		m_Object->MakeChildOf(root->Object());
		m_Parent = nullptr;
	}

	void UIelement::SetParent(UIelement* parent)
	{
		m_Parent = parent;
		m_Object->MakeChildOf(m_Parent->Object());
	}

}