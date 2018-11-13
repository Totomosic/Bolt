#include "Input.h"

namespace Bolt
{

	MouseInstance Input::s_Mouse = MouseInstance();
	KeyboardInstance Input::s_Keyboard = KeyboardInstance();
	Window* Input::s_Window = nullptr;

	std::vector<char> Input::s_PressedCharacters = std::vector<char>();
	std::vector<KeyboardInstance::Key> Input::s_ChangedKeys = std::vector<KeyboardInstance::Key>();
	std::vector<MouseInstance::Button> Input::s_ChangedButtons = std::vector<MouseInstance::Button>();

	const MouseInstance& Input::Mouse()
	{
		return s_Mouse;
	}

	const KeyboardInstance& Input::Keyboard()
	{
		return s_Keyboard;
	}

	const std::vector<char>& Input::PressedCharacters()
	{
		return s_PressedCharacters;
	}

	Vector3f Input::MousePosition()
	{
		return Vector3f(s_Mouse.X, s_Mouse.Y, 0);
	}

	Vector3f Input::RelMousePosition()
	{
		return Vector3f(s_Mouse.RelX, s_Mouse.RelY, 0);
	}

	Vector3f Input::NormalizedMousePosition()
	{
		return Vector3f(Map<float>((float)s_Mouse.X, 0, (float)s_Window->Width(), -1, 1), Map<float>((float)s_Mouse.Y, 0, (float)s_Window->Height(), -1, 1), 0);
	}

	Vector3f Input::RelNormalizedMousePosition()
	{
		return Vector3f(Map<float>((float)s_Mouse.RelX, 0, (float)s_Window->Width(), -1, 1), Map<float>((float)s_Mouse.RelY, 0, (float)s_Window->Height(), -1, 1), 0);
	}

	Vector2f Input::MouseScroll()
	{
		return Vector2f(s_Mouse.XScroll, s_Mouse.YScroll);
	}

	Vector2f Input::RelMouseScroll()
	{
		return Vector2f(s_Mouse.RelXScroll, s_Mouse.RelYScroll);
	}

	bool Input::MouseButtonDown(MouseButton mouseButton, KeyMod mods)
	{
		return s_Mouse.Buttons[(int)mouseButton].State == ButtonState::Pressed && TestKeyMods(mods);
	}

	bool Input::MouseButtonPressed(MouseButton mouseButton, KeyMod mods)
	{
		MouseInstance::Button* button = GetMouseButton(mouseButton);
		if (button == nullptr)
		{
			return false;
		}
		return button->State == ButtonState::Pressed && TestKeyMods(mods);
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
		return button->State == ButtonState::Released && TestKeyMods(mods);
	}

	bool Input::KeyDown(Keycode key, KeyMod mods)
	{
		return s_Keyboard.Keys[(int)key].State == ButtonState::Pressed && TestKeyMods(mods);
	}

	bool Input::KeyPressed(Keycode key, KeyMod mods)
	{
		KeyboardInstance::Key* k = GetKey(key);
		if (k == nullptr)
		{
			return false;
		}
		return k->State == ButtonState::Pressed && TestKeyMods(mods);
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
		return k->State == ButtonState::Released && TestKeyMods(mods);
	}

	void Input::HideCursor()
	{
		glfwSetInputMode(s_Window->WindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		s_Mouse.IsVisible = false;
	}

	void Input::ShowCursor()
	{
		glfwSetInputMode(s_Window->WindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		s_Mouse.IsVisible = true;
	}

	KeyboardInstance::Key* Input::GetKey(Keycode keycode)
	{
		for (KeyboardInstance::Key& button : s_ChangedKeys)
		{
			if (button.KeyID == keycode)
			{
				return &button;
			}
		}
		return nullptr;	
	}

	MouseInstance::Button* Input::GetMouseButton(MouseButton mouseButton)
	{
		for (MouseInstance::Button& button : s_ChangedButtons)
		{
			if (button.ButtonID == mouseButton)
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
	
	void Input::Initialize(Window* window)
	{
		s_Window = window;
		for (int i = 0; i < BLT_MAX_KEYS; i++)
		{
			s_Keyboard.Keys[i].KeyID = (Keycode)i;
			s_Keyboard.Keys[i].State = ButtonState::Released;
		}
		for (int i = 0; i < BLT_MAX_MOUSE_BUTTONS; i++)
		{
			s_Mouse.Buttons[i].ButtonID = (MouseButton)i;
			s_Mouse.Buttons[i].State = ButtonState::Released;
		}
		s_Mouse.Left = ButtonState::Released;
		s_Mouse.Middle = ButtonState::Released;
		s_Mouse.Right = ButtonState::Released;
		s_Mouse.X = 0;
		s_Mouse.Y = 0;
		s_Mouse.RelX = 0;
		s_Mouse.RelY = 0;
		s_Mouse.XScroll = 0;
		s_Mouse.YScroll = 0;
		s_Mouse.RelXScroll = 0;
		s_Mouse.RelYScroll = 0;
		s_Mouse.IsVisible = true;
	}

	void Input::Terminate()
	{
		s_Mouse = MouseInstance();
		s_Keyboard = KeyboardInstance();
		s_ChangedButtons.clear();
		s_ChangedKeys.clear();
		s_Window = nullptr;
	}

	void Input::Update()
	{
		s_PressedCharacters.clear();
		s_ChangedButtons.clear();
		s_ChangedKeys.clear();
		s_Mouse.RelX = 0;
		s_Mouse.RelY = 0;
		s_Mouse.RelXScroll = 0;
		s_Mouse.RelYScroll = 0;
	}

	void Input::MousePositionCallback(GLFWwindow* window, double mouseX, double mouseY)
	{
		mouseY = (double)s_Window->Height() - mouseY;
		s_Mouse.RelX = (float)mouseX - s_Mouse.X;
		s_Mouse.RelY = (float)mouseY - s_Mouse.Y;
		s_Mouse.X = (float)mouseX;
		s_Mouse.Y = (float)mouseY;
	}

	void Input::MouseScrollCallback(GLFWwindow* window, double xScroll, double yScroll)
	{
		s_Mouse.RelXScroll = (float)xScroll;
		s_Mouse.RelYScroll = (float)yScroll;
		s_Mouse.XScroll += (float)xScroll;
		s_Mouse.YScroll += (float)yScroll;
	}

	void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action != GLFW_REPEAT)
		{
			s_Mouse.Buttons[button].State = (ButtonState)action;
			s_ChangedButtons.push_back({ (MouseButton)button, (ButtonState)action });
			std::unique_ptr<MouseEventArgs> args = std::make_unique<MouseEventArgs>();
			args->Button = (MouseButton)button;
			if ((ButtonState)action == ButtonState::Pressed)
			{
				EventManager::Post(Events::MOUSE_PRESSED, std::move(args));
			}
			else
			{
				EventManager::Post(Events::MOUSE_RELEASED, std::move(args));
			}
		}
	}

	void Input::KeyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action != GLFW_REPEAT)
		{
			s_Keyboard.Keys[key].State = (ButtonState)action;
			s_ChangedKeys.push_back({ (Keycode)key, (ButtonState)action });
			std::unique_ptr<KeyEventArgs> args = std::make_unique<KeyEventArgs>();
			args->Key = (Keycode)key;
			if ((ButtonState)action == ButtonState::Pressed)
			{
				EventManager::Post(Events::KEY_PRESSED, std::move(args));
			}
			else
			{
				EventManager::Post(Events::KEY_RELEASED, std::move(args));
			}
		}
	}

	void Input::WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		s_Window->SetSize(width, height);
	}

	void Input::CharPressedCallback(GLFWwindow* window, uint code)
	{
		s_PressedCharacters.push_back((char)code);
	}

}