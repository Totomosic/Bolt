#include "bltpch.h"
#include "UITextInput.h"
#include "../UIManager.h"

namespace Bolt
{

	UITextInput::UITextInput(UIManager* manager, UIElement* parent, float width, float height, const ResourcePtr<Font>& font, const Color& fontColor, std::unique_ptr<Material>&& backgroundMaterial, Transform&& transform) : UICompoundElement(manager, parent),
		m_Background(nullptr), m_Text(nullptr), m_String(""), m_Obfuscate(false)
	{
		SetGameObject(m_Manager->Factory().Instantiate(std::move(transform)));
		m_Background = &CreateSurface(width, height, std::move(backgroundMaterial));
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
		return m_String;
	}

	bool UITextInput::IsObfuscating() const
	{
		return m_Obfuscate;
	}

	void UITextInput::SetObfuscating(bool enabled)
	{
		bool different = m_Obfuscate != enabled;
		m_Obfuscate = enabled;
		if (different)
		{
			TextElement().SetText(GetTextRepresentation());
		}
	}

	void UITextInput::SetupEventHandlers()
	{
		Events().OnCharPressed.AddEventListener([this](Event<UICharPressedEvent>& e)
			{
				m_String += (char)e.Data.CharCode;
				TextElement().SetText(GetTextRepresentation());
				e.StopPropagation();
			});
		Events().OnKeyDown.AddEventListener([this](Event<UIKeyDownEvent>& e)
			{
				if (e.Data.Key == Keycode::Backspace && GetText().size() > 0)
				{
					m_String.pop_back();
					TextElement().SetText(GetTextRepresentation());
				}
				e.StopPropagation();
			});
	}

	blt::string UITextInput::GetTextRepresentation() const
	{
		if (m_Obfuscate)
		{
			blt::string result = "";
			for (int i = 0; i < m_String.size(); i++)
			{
				result += '*';
			}
			return result;
		}
		return m_String;
	}

}