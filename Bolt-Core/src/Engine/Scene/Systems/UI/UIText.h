#pragma once
#include "UIElement.h"

namespace Bolt
{

	class BLT_API UIText : public UIElement
	{
	private:
		blt::string m_Text;
		Color m_Color;
		ResourcePtr<Font> m_Font;
		AlignH m_Horizontal;
		AlignV m_Vertical;
		
	public:
		UIText(UIManager* manager, UIElement* parent, const blt::string& text, const ResourcePtr<Font>& font, const Color& color,
			Transform transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);

		const blt::string& GetText() const;
		const ResourcePtr<Font>& GetFont() const;
		const Color& GetColor() const;

		void SetColor(const Color& color);
		void SetText(const blt::string& text);
		void SetFont(const ResourcePtr<Font>& font);

	private:
		void UpdateModel();
		void UpdateMaterial();
	};

}