#include "Types.h"
#include "Text.h"

namespace Bolt
{

	Text::Text(const blt::string& text, const ResourcePtr<const Bolt::Font>& font, const Color& color, Transform&& transform, AlignH horizontal, AlignV vertical) : UIelement(),
		m_String(text), m_Font(font), m_Color(color), m_Transform(std::move(transform)), m_AlignH(horizontal), m_AlignV(vertical)
	{
		
	}

	const blt::string& Text::String() const
	{
		return m_String;
	}

	ResourcePtr<const Font> Text::Font() const
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

	void Text::SetFont(const ResourcePtr<const Bolt::Font>& font)
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
		Mesh& mesh = m_GameObject->Components().GetComponent<MeshRenderer>().Mesh;
		mesh.Materials[0].BaseColor = m_Color;
	}

	void Text::CreateGameObject()
	{
		SetGameObject(GameObject::Instantiate(Parent()->Object()->GetLayer(), std::move(m_Transform)));
		Mesh mesh;
		mesh.Models.push_back({ ResourcePtr<const Model>(new Model(TextFactory(m_String, m_Font, Color::White, m_AlignH, m_AlignV)), true), Matrix4f::Identity(), { 0 } });
		mesh.Materials[0].BaseColor = m_Color;
		mesh.Materials[0].Shader = Shader::DefaultFont();
		mesh.Materials[0].Textures.Textures.push_back(m_Font);
		mesh.Materials[0].RenderOptions.DepthFunc = DepthFunction::Lequal;
		m_GameObject->Components().AddComponent(std::make_unique<MeshRenderer>(std::move(mesh)));
	}

	void Text::CreateTextModel()
	{
		Mesh& mesh = m_GameObject->Components().GetComponent<MeshRenderer>().Mesh;
		mesh.Models[0].Model = ResourcePtr<const Model>(new Model(TextFactory(m_String, m_Font, Color::White, m_AlignH, m_AlignV)), true);
	}

}