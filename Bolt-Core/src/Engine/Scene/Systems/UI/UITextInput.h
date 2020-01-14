#pragma once
#include "UIText.h"
#include "Shapes/UIRectangle.h"

namespace Bolt
{

	class BLT_API UITextInput : public UIElement
	{
	private:
		UIText* m_Text;
		UIElement* m_Background;
		std::string m_String;
		bool m_Obfuscate;

	public:
		UITextInput(UIManager* manager, UIElement* parent, float width, float height, const ResourcePtr<Font>& font, const Color& fontColor,
			std::unique_ptr<Material>&& backgroundMaterial, Transform transform = Transform());
		UITextInput(UIManager* manager, UIElement* parent, std::unique_ptr<UIElement>&& backgroundElement, const ResourcePtr<Font>& font, const Color& fontColor, Transform transform = Transform());

		UIText& GetTextElement() const;
		UIElement& GetBackgroundElement() const;

		const std::string& GetText() const;
		std::string GetTextRepresentation() const;
		bool IsObfuscated() const;
		void SetObfuscated(bool enabled);

		void SetText(const std::string& text);
	};

}