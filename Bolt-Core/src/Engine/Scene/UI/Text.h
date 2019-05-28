#pragma once
#include "UIelement.h"
#include "Graphics/Resources/Textures/Fonts/Font.h"

namespace Bolt
{

	class BLT_API Text : public UIelement
	{
	private:
		blt::string m_String;
		ResourcePtr<Bolt::Font> m_Font;
		Color m_Color;
		AlignH m_AlignH;
		AlignV m_AlignV;

		Transform m_Transform;

	public:
		Text(const blt::string& text, const ResourcePtr<Bolt::Font>& font, const Color& color = Color::White, Transform&& transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center);

		const blt::string& String() const;
		ResourcePtr<Bolt::Font> Font() const;
		const Color& TextColor() const;

		void SetText(const blt::string& text);
		void SetFont(const ResourcePtr<Bolt::Font>& font);
		void SetColor(const Color& color);

	protected:
		virtual void SetUIroot(UIroot* root) override;

	private:
		void CreateTextModel();

	};

}