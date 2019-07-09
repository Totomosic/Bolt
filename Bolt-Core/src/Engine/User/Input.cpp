#include "bltpch.h"

#include "Input.h"
#include "Engine/Engine.h"

namespace Bolt
{

	Input::Input(Window* window)
		: m_EventBus(), m_Mouse(), m_Keyboard(), m_Window(window), m_PressedCharacters(), m_ChangedKeys(), m_ChangedButtons(), 
		OnKeyPressed(m_EventBus.GetEmitter<KeyPressedEvent>(Events::Input.KeyPressed)), 
		OnKeyReleased(m_EventBus.GetEmitter<KeyReleasedEvent>(Events::Input.KeyReleased)),
		OnMousePressed(m_EventBus.GetEmitter<MousePressedEvent>(Events::Input.MousePressed)),
		OnMouseReleased(m_EventBus.GetEmitter<MouseReleasedEvent>(Events::Input.MouseReleased)),
		OnMouseMoved(m_EventBus.GetEmitter<MouseMovedEvent>(Events::Input.MouseMoved)),
		OnMouseScrolled(m_EventBus.GetEmitter<MouseScrolledEvent>(Events::Input.MouseScrolled))
	{
		for (int i = 0; i < BLT_MAX_KEYS; i++)
		{
			m_Keyboard.Keys[i].KeyID = i;
			m_Keyboard.Keys[i].State = (int)ButtonState::Released;
		}
		for (int i = 0; i < BLT_MAX_MOUSE_BUTTONS; i++)
		{
			m_Mouse.Buttons[i].ButtonID = i;
			m_Mouse.Buttons[i].State = (int)ButtonState::Released;
		}
		m_Mouse.Left = ButtonState::Released;
		m_Mouse.Middle = ButtonState::Released;
		m_Mouse.Right = ButtonState::Released;
		m_Mouse.X = 0;
		m_Mouse.Y = 0;
		m_Mouse.RelX = 0;
		m_Mouse.RelY = 0;
		m_Mouse.XScroll = 0;
		m_Mouse.YScroll = 0;
		m_Mouse.RelXScroll = 0;
		m_Mouse.RelYScroll = 0;
		m_Mouse.IsVisible = true;
	}

	Input& Input::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetInput();
	}

	const MouseInstance& Input::Mouse()
	{
		return m_Mouse;
	}

	const KeyboardInstance& Input::Keyboard()
	{
		return m_Keyboard;
	}

	const std::vector<char>& Input::PressedCharacters()
	{
		return m_PressedCharacters;
	}

	Vector3f Input::MousePosition()
	{
		return Vector3f(m_Mouse.X, m_Mouse.Y, 0);
	}

	Vector3f Input::MousePosition(float width, float height)
	{
		return Vector3f(Map<float>(m_Mouse.X, 0, m_Window->Width(), 0, width), Map<float>(m_Mouse.Y, 0, m_Window->Height(), 0, height), 0);
	}

	Vector3f Input::RelMousePosition()
	{
		return Vector3f(m_Mouse.RelX, m_Mouse.RelY, 0);
	}

	Vector3f Input::RelMousePosition(float width, float height)
	{
		return Vector3f(Map<float>(m_Mouse.RelX, -m_Window->Width(), m_Window->Width(), -width, width), Map<float>(m_Mouse.RelY, -m_Window->Height(), m_Window->Height(), -height, height), 0);
	}

	Vector3f Input::NormalizedMousePosition()
	{
		return Vector3f(Map<float>((float)m_Mouse.X, 0, (float)m_Window->Width(), -1, 1), Map<float>((float)m_Mouse.Y, 0, (float)m_Window->Height(), -1, 1), 0);
	}

	Vector3f Input::RelNormalizedMousePosition()
	{
		return Vector3f(Map<float>((float)m_Mouse.RelX, 0, (float)m_Window->Width(), -1, 1), Map<float>((float)m_Mouse.RelY, 0, (float)m_Window->Height(), -1, 1), 0);
	}

	Vector2f Input::MouseScroll()
	{
		return Vector2f(m_Mouse.XScroll, m_Mouse.YScroll);
	}

	Vector2f Input::RelMouseScroll()
	{
		return Vector2f(m_Mouse.RelXScroll, m_Mouse.RelYScroll);
	}

	bool Input::MouseButtonDown(MouseButton mouseButton, KeyMod mods)
	{
		return m_Mouse.Buttons[(int)mouseButton].State == (int)ButtonState::Pressed && TestKeyMods(mods);
	}

	bool Input::MouseButtonPressed(MouseButton mouseButton, KeyMod mods)
	{
		MouseInstance::Button* button = GetMouseButton(mouseButton);
		if (button == nullptr)
		{
			return false;
		}
		return button->State == (int)ButtonState::Pressed && TestKeyMods(mods);
	}

	bool Input::MouseButtonUp(MouseButton mouseButton, KeyMod mods)
	{
		return !MouseButtonDown(mouseButton) && TestKeyMods(mods);
	}

	bool Input::MouseButtonReleased(MouseButton mouseButton, KeyMod mods)
	{
		MouseInstance::Button* button = GetMouseButton(mouseButton);
		if (button == nullptr)
		{
			return false;
		}
		return button->State == (int)ButtonState::Released && TestKeyMods(mods);
	}

	bool Input::KeyDown(Keycode key, KeyMod mods)
	{
		return m_Keyboard.Keys[(int)key].State == (int)ButtonState::Pressed && TestKeyMods(mods);
	}

	bool Input::KeyPressed(Keycode key, KeyMod mods)
	{
		KeyboardInstance::Key* k = GetKey(key);
		if (k == nullptr)
		{
			return false;
		}
		return k->State == (int)ButtonState::Pressed && TestKeyMods(mods);
	}

	bool Input::KeyUp(Keycode key, KeyMod mods)
	{
		return !KeyDown(key) && TestKeyMods(mods);
	}

	bool Input::KeyReleased(Keycode key, KeyMod mods)
	{
		KeyboardInstance::Key* k = GetKey(key);
		if (k == nullptr)
		{
			return false;
		}
		return k->State == (int)ButtonState::Released && TestKeyMods(mods);
	}

	void Input::HideCursor()
	{
		glfwSetInputMode((GLFWwindow*)m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_Mouse.IsVisible = false;
	}

	void Input::ShowCursor()
	{
		glfwSetInputMode((GLFWwindow*)m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_Mouse.IsVisible = true;
	}

	KeyboardInstance::Key* Input::GetKey(Keycode keycode)
	{
		for (KeyboardInstance::Key& button : m_ChangedKeys)
		{
			if (button.KeyID == (int)keycode)
			{
				return &button;
			}
		}
		return nullptr;	
	}

	MouseInstance::Button* Input::GetMouseButton(MouseButton mouseButton)
	{
		for (MouseInstance::Button& button : m_ChangedButtons)
		{
			if (button.ButtonID == (int)mouseButton)
			{
				return &button;
			}
		}
		return nullptr;
	}

	bool Input::TestKeyMods(KeyMod mods)
	{
		if (BLT_CONTAINS_BIT(mods, KeyMod::Ignore))
		{
			return true;
		}
		bool value = true;
		bool wasSet = false;
		if (BLT_CONTAINS_BIT(mods, KeyMod::LShift))
		{
			value = value && KeyDown(Keycode::LShift);
			wasSet = true;
		}
		if (BLT_CONTAINS_BIT(mods, KeyMod::RShift))
		{
			value = value && KeyDown(Keycode::RShift);
			wasSet = true;
		}
		if (BLT_CONTAINS_BIT(mods, KeyMod::Shift))
		{
			value = value && (KeyDown(Keycode::LShift) || KeyDown(Keycode::RShift));
			wasSet = true;
		}
		if (BLT_CONTAINS_BIT(mods, KeyMod::LAlt))
		{
			value = value && KeyDown(Keycode::LAlt);
			wasSet = true;
		}
		if (BLT_CONTAINS_BIT(mods, KeyMod::RAlt))
		{
			value = value && KeyDown(Keycode::RAlt);
			wasSet = true;
		}
		if (BLT_CONTAINS_BIT(mods, KeyMod::Alt))
		{
			value = value && (KeyDown(Keycode::LAlt) || KeyDown(Keycode::RAlt));
			wasSet = true;
		}
		if (BLT_CONTAINS_BIT(mods, KeyMod::LCtrl))
		{
			value = value && KeyDown(Keycode::LCtrl);
			wasSet = true;
		}
		if (BLT_CONTAINS_BIT(mods, KeyMod::RCtrl))
		{
			value = value && KeyDown(Keycode::RCtrl);
			wasSet = true;
		}
		if (BLT_CONTAINS_BIT(mods, KeyMod::Ctrl))
		{
			value = value && (KeyDown(Keycode::LCtrl) || KeyDown(Keycode::RCtrl));
			wasSet = true;
		}
		if (BLT_CONTAINS_BIT(mods, KeyMod::None))
		{
			return !wasSet;
		}
		return value;
	}

	void Input::Update()
	{
		m_PressedCharacters.clear();
		m_ChangedButtons.clear();
		m_ChangedKeys.clear();
		m_Mouse.RelX = 0;
		m_Mouse.RelY = 0;
		m_Mouse.RelXScroll = 0;
		m_Mouse.RelYScroll = 0;
	}

	bool Input::IsCurrentlySelected() const
	{
		return true;
	}

	void Input::MousePositionCallback(double mouseX, double mouseY)
	{
		if (IsCurrentlySelected())
		{
			mouseY = (double)m_Window->Height() - mouseY;
			m_Mouse.RelX = (float)mouseX - m_Mouse.X;
			m_Mouse.RelY = (float)mouseY - m_Mouse.Y;
			m_Mouse.X = (float)mouseX;
			m_Mouse.Y = (float)mouseY;
			MouseMovedEvent args;
			args.x = (float)mouseX;
			args.y = (float)mouseY;
			args.relX = m_Mouse.RelX;
			args.relY = m_Mouse.RelY;
			OnMouseMoved.Emit(std::move(args));
		}
	}

	void Input::MouseScrollCallback(double xScroll, double yScroll)
	{
		if (IsCurrentlySelected())
		{
			m_Mouse.RelXScroll = (float)xScroll;
			m_Mouse.RelYScroll = (float)yScroll;
			m_Mouse.XScroll += (float)xScroll;
			m_Mouse.YScroll += (float)yScroll;
			MouseScrolledEvent args;
			args.xOffset = (float)xScroll;
			args.yOffset = (float)yScroll;
			OnMouseScrolled.Emit(std::move(args));
		}
	}

	void Input::MouseButtonCallback(int button, int action, int mods)
	{
		if (IsCurrentlySelected())
		{
			if (action != GLFW_REPEAT)
			{
				m_Mouse.Buttons[button].State = action;
				m_ChangedButtons.push_back({ button, action });
			}
			if (action == (int)ButtonState::Pressed || action == GLFW_REPEAT)
			{
				MousePressedEvent args;
				args.Button = (MouseButton)button;
				args.IsRepeat = action == GLFW_REPEAT;
				args.x = m_Mouse.X;
				args.y = m_Mouse.Y;
				OnMousePressed.Emit(std::move(args));
			}
			else
			{
				MouseReleasedEvent args;
				args.Button = (MouseButton)button;
				args.x = m_Mouse.X;
				args.y = m_Mouse.Y;
				OnMouseReleased.Emit(std::move(args));
			}
		}
	}

	void Input::KeyboardKeyCallback(int key, int scancode, int action, int mods)
	{
		if (IsCurrentlySelected())
		{
			if (action != GLFW_REPEAT)
			{
				m_Keyboard.Keys[key].State = action;
				m_ChangedKeys.push_back(KeyboardInstance::Key{ key, action });
			}
			if (action == (int)ButtonState::Pressed || action == GLFW_REPEAT)
			{
				KeyPressedEvent args;
				args.KeyCode = (Keycode)key;
				args.IsRepeat = action == GLFW_REPEAT;
				OnKeyPressed.Emit(std::move(args));
			}
			else
			{
				KeyReleasedEvent args;
				args.KeyCode = (Keycode)key;
				OnKeyReleased.Emit(std::move(args));
			}
		}
	}

	void Input::CharPressedCallback(uint32_t code)
	{
		if (IsCurrentlySelected())
		{
			m_PressedCharacters.push_back((char)code);
		}
	}

}