#include "bltpch.h"
#include "UIText.h"

#include "Graphics/Assets/AssetManager.h"

namespace Bolt
{

	UIText::UIText(UIManager* manager, UIElement* parent, const std::string& text, const AssetHandle<Font>& font, const Color& color, Transform transform, AlignH horizontal, AlignV vertical)
		: UIElement(manager, parent), m_Text(text), m_Color(color), m_Font(font), m_Horizontal(horizontal), m_Vertical(vertical)
	{
		EntityHandle entity = GetEntityFactory().Text(text, font, color, std::move(transform), horizontal, vertical);
		SetEntity(entity);
	}

	const std::string& UIText::GetText() const
	{
		return m_Text;
	}

	const AssetHandle<Font>& UIText::GetFont() const
	{
		return m_Font;
	}

	const Color& UIText::GetColor() const
	{
		return m_Color;
	}

	void UIText::SetColor(const Color& color)
	{
		m_Color = color;
		UpdateMaterial();
	}

	void UIText::SetText(const std::string& text)
	{
		m_Text = text;
		UpdateModel();
	}

	void UIText::SetFont(const AssetHandle<Font>& font)
	{
		m_Font = font;
		UpdateModel();
		UpdateMaterial();
	}

	void UIText::UpdateModel()
	{
		Model* model = new Model(TextFactory(m_Text, m_Font, Color::White, m_Horizontal, m_Vertical), false);
		GetMesh().Models[0].Model = AssetHandle<Model>(model, true);
	}

	void UIText::UpdateMaterial()
	{
		std::unique_ptr<Material> material = AssetManager::Get().Materials().Font(m_Font, m_Color);
		GetMesh().Materials[0] = std::move(material);
	}

}