#include "bltpch.h"
#include "UITextInput.h"
#include "../UIManager.h"

namespace Bolt
{

	UITextInput::UITextInput(UIManager* manager, UIElement* parent, float width, float height, const ResourcePtr<Font>& font, const Color& fontColor, std::unique_ptr<Material>&& backgroundMaterial, Transform transform)
		: UITextInput(manager, parent, std::make_unique<UIRectangle>(manager, nullptr, width, height, std::move(backgroundMaterial)), font, fontColor, std::move(transform))
	{
		
	}

	UITextInput::UITextInput(UIManager* manager, UIElement* parent, std::unique_ptr<UIElement>&& backgroundElement, const ResourcePtr<Font>& font, const Color& fontColor, Transform transform)
		: UIElement(manager, parent), m_Text(nullptr), m_Background(nullptr), m_String(""), m_Obfuscate(false)
	{
		SetEntity(GetEntityFactory().CreateTransform(std::move(transform)));
		m_Background = &AddChild(std::move(backgroundElement));
		m_Text = &CreateText(m_String, font, fontColor, Transform({ 0, 0, 1 }), AlignH::Center, AlignV::Center);
		m_Background->SetCompoundElement(this);
		m_Text->SetCompoundElement(this);

		Events().OnCharPressed().AddEventListener([this](Event<UI<CharPressedEvent>>& e)
			{
				m_String += (char)e.Data.OriginalEvent.CharCode;
				GetTextElement().SetText(GetTextRepresentation());
				e.StopPropagation();
			});
		Events().OnKeyDown().AddEventListener([this](Event<UI<KeyDownEvent>>& e)
			{
				if (e.Data.OriginalEvent.KeyCode == Keycode::Backspace && GetText().size() > 0)
				{
					m_String.pop_back();
					GetTextElement().SetText(GetTextRepresentation());
					e.StopPropagation();
				}
			});
	}

	UIText& UITextInput::GetTextElement() const
	{
		return *m_Text;
	}

	UIElement& UITextInput::GetBackgroundElement() const
	{
		return *m_Background;
	}

	const std::string& UITextInput::GetText() const
	{
		return m_String;
	}

	std::string UITextInput::GetTextRepresentation() const
	{
		if (!m_Obfuscate)
		{
			return GetText();
		}
		std::string result;
		result.resize(m_String.size(), '*');
		return result;
	}

	bool UITextInput::IsObfuscated() const
	{
		return m_Obfuscate;
	}

	void UITextInput::SetObfuscated(bool enabled)
	{
		if (enabled != m_Obfuscate)
		{
			m_Obfuscate = enabled;
			GetTextElement().SetText(GetTextRepresentation());
		}
	}

	void UITextInput::SetText(const std::string& text)
	{
		if (text != m_String)
		{
			m_String = text;
			GetTextElement().SetText(GetTextRepresentation());
		}
	}

}