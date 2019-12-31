#include "bltpch.h"
#include "UIFactory.h"
#include "../UIManager.h"
#include "Graphics/Resources/ResourceManager.h"

#include "UIElement.h"
#include "UIText.h"
#include "Shapes/UIRectangle.h"

namespace Bolt
{

	UIFactory::UIFactory(UIManager* manager, UIElement& parentElement)
		: m_Manager(manager), m_ParentElement(parentElement)
	{
	}

	UIElement& UIFactory::CreateElement(Transform transform)
	{
		return m_ParentElement.AddChild(std::make_unique<UIElement>(m_Manager, &m_ParentElement, m_Manager->GetFactory().CreateTransform(std::move(transform))));
	}

	UIRectangle& UIFactory::CreateRectangle(float width, float height, std::unique_ptr<Material>&& material, Transform transform)
	{
		return (UIRectangle&)m_ParentElement.AddChild(std::make_unique<UIRectangle>(m_Manager, &m_ParentElement, width, height, std::move(material), std::move(transform)));
	}

	UIRectangle& UIFactory::CreateRectangle(float width, float height, const Color& color, Transform transform)
	{
		return CreateRectangle(width, height, ResourceManager::Get().Materials().Default(color), std::move(transform));
	}

	UIRectangle& UIFactory::CreateImage(float width, float height, const ResourcePtr<Texture2D>& image, Transform transform)
	{
		return CreateRectangle(width, height, ResourceManager::Get().Materials().Texture(image), std::move(transform));
	}

	UIText& UIFactory::CreateText(const blt::string& text, const ResourcePtr<Font>& font, const Color& color, Transform transform, AlignH horizontal, AlignV vertical)
	{
		return (UIText&)m_ParentElement.AddChild(std::make_unique<UIText>(m_Manager, &m_ParentElement, text, font, color, std::move(transform), horizontal, vertical));
	}

	UIText& UIFactory::CreateText(const blt::string& text, const Color& color, Transform transform, AlignH horizontal, AlignV vertical)
	{
		return CreateText(text, ResourceManager::Get().Fonts().Default(), color, std::move(transform), horizontal, vertical);
	}

}