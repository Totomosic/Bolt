#pragma once
#include "UIElement.h"

namespace Bolt
{

	class BLT_API UIText : public UIElement
	{
	private:
		ResourcePtr<Font> m_Font;
		blt::string m_Text;
		AlignH m_HorizontalAlign;
		AlignV m_VerticalAlign;

	public:
		UIText(UIManager* manager, UIElement* parent, const blt::string& text, const ResourcePtr<Font>& font, const Color& color, Transform&& transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);

		const blt::string& GetText() const;
		const ResourcePtr<Font>& GetFont() const;

		void SetColor(const Color& color);
		void SetText(const blt::string& text);
		void SetFont(const ResourcePtr<Font>& font);

	private:
		void RecreateModel();
		Mesh CreateMesh(const Color& color);

	};

}