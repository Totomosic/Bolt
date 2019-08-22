#include "bltpch.h"
#include "UITextInput.h"
#include "../UIManager.h"

namespace Bolt
{

	UITextInput::UITextInput(UIManager* manager, UIElement* parent, float width, float height, const ResourcePtr<Font>& font, const Color& fontColor, std::unique_ptr<Material>&& backgroundMaterial, Transform&& transform) : UICompoundElement(manager, parent),
		m_Background(nullptr), m_Text(nullptr)
	{
		m_Background = &CreateSurface(width, height, std::move(backgroundMaterial), std::move(transform));
		m_Text = &CreateText("", font, fontColor, Transform({ 0, 0, 1 }), AlignH::Center, AlignV::Center);
		SetupEventHandlers();
	}

	UISurface& UITextInput::BackgroundElement() const
	{
		return *m_Background;
	}

	UIText& UITextInput::TextElement() const
	{
		return *m_Text;
	}

	const blt::string& UITextInput::GetText() const
	{
		return m_Text->GetText();
	}

	void UITextInput::SetupEventHandlers()
	{
		Events().OnCharPressed.AddEventListener([this](Event<UICharPressedEvent>& e)
			{
				TextElement().SetText(TextElement().GetText() + (char)e.Data.CharCode);
				e.StopPropagation();
			});
		Events().OnKeyDown.AddEventListener([this](Event<UIKeyDownEvent>& e)
			{
				if (e.Data.Key == Keycode::Backspace && GetText().size() > 0)
				{
					TextElement().SetText(GetText().substr(0, GetText().length() - 1));
					e.StopPropagation();
				}
			});
	}

}