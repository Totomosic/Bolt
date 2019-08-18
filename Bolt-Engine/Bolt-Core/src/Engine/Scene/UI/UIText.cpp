#include "bltpch.h"
#include "UIText.h"
#include "UIManager.h"
#include "Graphics/Resources/ResourceManager.h"

namespace Bolt
{

	UIText::UIText(UIManager* manager, UIElement* parent, const blt::string& text, const ResourcePtr<Font>& font, const Color& color, Transform&& transform, AlignH horizontal, AlignV vertical) : UIElement(manager, parent),
		m_Text(text), m_Font(font), m_HorizontalAlign(horizontal), m_VerticalAlign(vertical)
	{
		SetGameObject(m_Manager->Factory().Instantiate(CreateMesh(color), std::move(transform)));
	}

	const blt::string& UIText::GetText() const
	{
		return m_Text;
	}

	const ResourcePtr<Font>& UIText::GetFont() const
	{
		return m_Font;
	}

	void UIText::SetColor(const Color& color)
	{
		Mesh& mesh = GetMesh();
		mesh.Materials[0] = ResourceManager::Get().Materials().Font(m_Font, color);
	}

	void UIText::SetText(const blt::string& text)
	{
		m_Text = text;
		RecreateModel();
	}

	void UIText::SetFont(const ResourcePtr<Font>& font)
	{
		m_Font = font;
		RecreateModel();
	}

	void UIText::RecreateModel()
	{
		Mesh& mesh = GetMesh();
		mesh.Models[0].Model = ResourcePtr<Model>(new Model(TextFactory(m_Text, m_Font, Color::White, m_HorizontalAlign, m_VerticalAlign)), true);
	}

	Mesh UIText::CreateMesh(const Color& color)
	{
		Mesh mesh;
		mesh.Models.push_back({ ResourcePtr<Model>(new Model(TextFactory(m_Text, m_Font, Color::White, m_HorizontalAlign, m_VerticalAlign)), true) });
		mesh.Materials.push_back(ResourceManager::Get().Materials().Font(m_Font, color));
		return mesh;
	}

}