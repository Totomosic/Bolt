#pragma once
#include "../UICompoundElement.h"
#include "../UIText.h"
#include "../UIsurface.h"

namespace Bolt
{

	class BLT_API UITextInput : public UICompoundElement
	{
	private:
		UISurface* m_Background;
		UIText* m_Text;
		blt::string m_String;
		bool m_Obfuscate;

	public:
		UITextInput(UIManager* manager, UIElement* parent, float width, float height, const ResourcePtr<Font>& font, const Color& fontColor, std::unique_ptr<Material>&& backgroundMaterial, Transform&& transform);

		UISurface& BackgroundElement() const;
		UIText& TextElement() const;

		const blt::string& GetText() const;
		bool IsObfuscating() const;
		void SetObfuscating(bool enabled);

	private:
		void SetupEventHandlers();
		blt::string GetTextRepresentation() const;

	};

}