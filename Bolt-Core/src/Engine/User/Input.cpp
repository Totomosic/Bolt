#include "Types.h"

#include "Input.h"

namespace Bolt
{

	MouseInstance Input::s_Mouse = MouseInstance();
	KeyboardInstance Input::s_Keyboard = KeyboardInstance();
	Window* Input::s_Window = nullptr;

	std::vector<char> Input::s_PressedCharacters = std::vector<char>();
	std::vector<KeyboardInstance::Key> Input::s_ChangedKeys = std::vector<KeyboardInstance::Key>();
	std::vector<MouseInstance::Button> Input::s_ChangedButtons = std::vector<MouseInstance::Button>();

	EventDispatcher<Bolt::KeyPressedEvent> Input::OnKeyPressed = EventDispatcher<Bolt::KeyPressedEvent>();
	EventDispatcher<Bolt::KeyReleasedEvent> Input::OnKeyReleased = EventDispatcher<Bolt::KeyReleasedEvent>();
	EventDispatcher<Bolt::MousePressedEvent> Input::OnMousePressed = EventDispatcher<Bolt::MousePressedEvent>();
	EventDispatcher<Bolt::MouseReleasedEvent> Input::OnMouseReleased = EventDispatcher<Bolt::MouseReleasedEvent>();
	EventDispatcher<Bolt::MouseScrolledEvent> Input::OnMouseScrolled = EventDispatcher<Bolt::MouseScrolledEvent>();
	EventDispatcher<Bolt::MouseMovedEvent> Input::OnMouseMoved = EventDispatcher<Bolt::MouseMovedEvent>();

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

	Vector3f Input::MousePosition(float width, float height)
	{
		return Vector3f(Map<float>(s_Mouse.X, 0, s_Window->Width(), 0, width), Map<float>(s_Mouse.Y, 0, s_Window->Height(), 0, height), 0);
	}

	Vector3f Input::RelMousePosition()
	{
		return Vector3f(s_Mouse.RelX, s_Mouse.RelY, 0);
	}

	Vector3f Input::RelMousePosition(float width, float height)
	{
		return Vector3f(Map<float>(s_Mouse.RelX, -s_Window->Width(), s_Window->Width(), -width, width), Map<float>(s_Mouse.RelY, -s_Window->Height(), s_Window->Height(), -height, height), 0);
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
		return s_Mouse.Buttons[(int)mouseButton].State == (int)ButtonState::Pressed && TestKeyMods(mods);
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
		return s_Keyboard.Keys[(int)key].State == (int)ButtonState::Pressed && TestKeyMods(mods);
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
			if (button.KeyID == (int)keycode)
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
	
	void Input::Initialize(Window* window)
	{
		s_Window = window;
		for (int i = 0; i < BLT_MAX_KEYS; i++)
		{
			s_Keyboard.Keys[i].KeyID = i;
			s_Keyboard.Keys[i].State = (int)ButtonState::Released;
		}
		for (int i = 0; i < BLT_MAX_MOUSE_BUTTONS; i++)
		{
			s_Mouse.Buttons[i].ButtonID = i;
			s_Mouse.Buttons[i].State = (int)ButtonState::Released;
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

		OnKeyPressed = EventDispatcher<Bolt::KeyPressedEvent>(Events::KEY_PRESSED);
		OnKeyReleased = EventDispatcher<Bolt::KeyReleasedEvent>(Events::KEY_RELEASED);
		OnMousePressed = EventDispatcher<Bolt::MousePressedEvent>(Events::MOUSE_PRESSED);
		OnMouseReleased = EventDispatcher<Bolt::MouseReleasedEvent>(Events::MOUSE_RELEASED);
		OnMouseScrolled = EventDispatcher<Bolt::MouseScrolledEvent>(Events::MOUSE_SCROLLED);
		OnMouseMoved = EventDispatcher<Bolt::MouseMovedEvent>(Events::MOUSE_POSITION_MOVED);
	}

	void Input::Terminate()
	{
		s_Mouse = MouseInstance();
		s_Keyboard = KeyboardInstance();
		s_ChangedButtons.clear();
		s_ChangedKeys.clear();
		s_Window = nullptr;

		OnKeyPressed.Destroy();
		OnKeyReleased.Destroy();
		OnMousePressed.Destroy();
		OnMouseReleased.Destroy();
		OnMouseScrolled.Destroy();
		OnMouseMoved.Destroy();
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
		std::unique_ptr<Bolt::MouseMovedEvent> args = std::make_unique<Bolt::MouseMovedEvent>();
		args->x = (float)mouseX;
		args->y = (float)mouseY;
		args->relX = s_Mouse.RelX;
		args->relY = s_Mouse.RelY;
		OnMouseMoved.Post(std::move(args));
	}

	void Input::MouseScrollCallback(GLFWwindow* window, double xScroll, double yScroll)
	{
		s_Mouse.RelXScroll = (float)xScroll;
		s_Mouse.RelYScroll = (float)yScroll;
		s_Mouse.XScroll += (float)xScroll;
		s_Mouse.YScroll += (float)yScroll;
		std::unique_ptr<Bolt::MouseScrolledEvent> args = std::make_unique<Bolt::MouseScrolledEvent>();
		args->xOffset = (float)xScroll;
		args->yOffset = (float)yScroll;
		OnMouseScrolled.Post(std::move(args));
	}

	void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action != GLFW_REPEAT)
		{
			s_Mouse.Buttons[button].State = action;
			s_ChangedButtons.push_back({ button, action });
		}
		if (action == (int)ButtonState::Pressed || action == GLFW_REPEAT)
		{
			std::unique_ptr<Bolt::MousePressedEvent> args = std::make_unique<Bolt::MousePressedEvent>();
			args->Button = (MouseButton)button;
			args->IsRepeat = action == GLFW_REPEAT;
			args->x = s_Mouse.X;
			args->y = s_Mouse.Y;
			OnMousePressed.Post(std::move(args));
		}
		else
		{
			std::unique_ptr<Bolt::MouseReleasedEvent> args = std::make_unique<Bolt::MouseReleasedEvent>();
			args->Button = (MouseButton)button;
			args->x = s_Mouse.X;
			args->y = s_Mouse.Y;
			OnMouseReleased.Post(std::move(args));
		}
	}

	void Input::KeyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action != GLFW_REPEAT)
		{
			s_Keyboard.Keys[key].State = action;
			s_ChangedKeys.push_back(KeyboardInstance::Key{ key, action });
		}
		if (action == (int)ButtonState::Pressed || action == GLFW_REPEAT)
		{
			std::unique_ptr<Bolt::KeyPressedEvent> args = std::make_unique<Bolt::KeyPressedEvent>();
			args->KeyCode = (Keycode)key;
			args->IsRepeat = action == GLFW_REPEAT;
			OnKeyPressed.Post(std::move(args));
		}
		else
		{
			std::unique_ptr<Bolt::KeyReleasedEvent> args = std::make_unique<Bolt::KeyReleasedEvent>();
			args->KeyCode = (Keycode)key;
			OnKeyReleased.Post(std::move(args));
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