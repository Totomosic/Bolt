#include "bltpch.h"

#include "Text.h"
#include "UIroot.h"
#include "Graphics/Resources/ResourceManager.h"

namespace Bolt
{

	Text::Text(const blt::string& text, const ResourcePtr<Bolt::Font>& font, const Color& color, Transform&& transform, AlignH horizontal, AlignV vertical) : UIelement(),
		m_String(text), m_Font(font), m_Color(color), m_Transform(std::move(transform)), m_AlignH(horizontal), m_AlignV(vertical)
	{
		
	}

	const blt::string& Text::String() const
	{
		return m_String;
	}

	ResourcePtr<Font> Text::Font() const
	{
		return m_Font;
	}

	const Color& Text::TextColor() const
	{
		return m_Color;
	}

	void Text::SetText(const blt::string& text)
	{
		if (text != String())
		{
			m_String = text;
			CreateTextModel();
		}
	}

	void Text::SetFont(const ResourcePtr<Bolt::Font>& font)
	{
		if (font != Font())
		{
			m_Font = font;
			CreateTextModel();
		}
	}

	void Text::SetColor(const Color& color)
	{
		m_Color = color;
		Mesh& mesh = m_Object->Components().GetComponent<MeshRenderer>().Mesh;
		mesh.Materials[0]->GetShader().GetLink("Color") = m_Color;
	}

	void Text::SetUIroot(UIroot* root)
	{
		UIelement::SetUIroot(root);
		Mesh mesh;
		mesh.Models.push_back({ ResourcePtr<const Model>(new Model(TextFactory(m_String, m_Font, Color::White, m_AlignH, m_AlignV)), true), Matrix4f::Identity(), { 0 } });
		mesh.Materials[0] = ResourceManager::Get().Materials().Font(m_Font, m_Color);
		m_Object->Components().AddComponent(std::make_unique<MeshRenderer>(std::move(mesh)));
		m_Object->transform() = std::move(m_Transform);
	}

	void Text::CreateTextModel()
	{
		Mesh& mesh = m_Object->Components().GetComponent<MeshRenderer>().Mesh;
		mesh.Models[0].Model = ResourcePtr<const Model>(new Model(TextFactory(m_String, m_Font, Color::White, m_AlignH, m_AlignV)), true);
	}

}