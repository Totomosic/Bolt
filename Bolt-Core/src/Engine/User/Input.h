#pragma once
#include "Bolt-Graphics.h"

#define BLT_MAX_KEYS 256
#define BLT_MAX_MOUSE_BUTTONS 32

namespace Bolt
{

	enum class MouseButton
	{
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,
		Right = GLFW_MOUSE_BUTTON_RIGHT
	};

	enum class ButtonState
	{
		Released = GLFW_RELEASE,
		Pressed = GLFW_PRESS
	};

	enum class Keycode
	{
		LShift = GLFW_KEY_LEFT_SHIFT,
		RShift = GLFW_KEY_RIGHT_SHIFT,
		LAlt = GLFW_KEY_LEFT_ALT,
		RAlt = GLFW_KEY_RIGHT_ALT,
		LCtrl = GLFW_KEY_LEFT_CONTROL,
		RCtrl = GLFW_KEY_RIGHT_CONTROL,
		Tab = GLFW_KEY_TAB,
		Enter = GLFW_KEY_ENTER,
		CapsLock = GLFW_KEY_CAPS_LOCK,
		Backspace = GLFW_KEY_BACKSPACE,

		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,
		Space = GLFW_KEY_SPACE,

		Grave = GLFW_KEY_GRAVE_ACCENT,
		Zero = GLFW_KEY_0,
		One = GLFW_KEY_1,
		Two = GLFW_KEY_2,
		Three = GLFW_KEY_3,
		Four = GLFW_KEY_4,
		Five = GLFW_KEY_5,
		Six = GLFW_KEY_6,
		Seven = GLFW_KEY_7,
		Eight = GLFW_KEY_8,
		Nine = GLFW_KEY_9,
		Minus = GLFW_KEY_MINUS,
		Equal = GLFW_KEY_EQUAL,

		Comma = GLFW_KEY_COMMA,
		Period = GLFW_KEY_PERIOD,
		Slash = GLFW_KEY_SLASH,
		Backslash = GLFW_KEY_BACKSLASH,
		Semicolon = GLFW_KEY_SEMICOLON,
		Apostrophe = GLFW_KEY_APOSTROPHE,
		OpenBracket = GLFW_KEY_LEFT_BRACKET,
		CloseBracket = GLFW_KEY_RIGHT_BRACKET,

		Insert = GLFW_KEY_INSERT,
		Home = GLFW_KEY_HOME,
		Delete = GLFW_KEY_DELETE,
		End = GLFW_KEY_END,
		PageUp = GLFW_KEY_PAGE_UP,
		PageDown = GLFW_KEY_PAGE_DOWN,

		Left = GLFW_KEY_LEFT,
		Right = GLFW_KEY_RIGHT,
		Up = GLFW_KEY_UP,
		Down = GLFW_KEY_DOWN,

		Esc = GLFW_KEY_ESCAPE,
		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12
	};

	enum class KeyMod
	{
		LShift = BLT_BIT(0),
		RShift = BLT_BIT(1),
		Shift = BLT_BIT(2),
		LAlt = BLT_BIT(3),
		RAlt = BLT_BIT(4),
		Alt = BLT_BIT(5),
		LCtrl = BLT_BIT(6),
		RCtrl = BLT_BIT(7),
		Ctrl = BLT_BIT(8),
		Ignore = BLT_BIT(9),
		None = BLT_BIT(10)
	};

	struct BLT_API MouseInstance
	{
	public:
		struct BLT_API Button
		{
			MouseButton ButtonID;
			ButtonState State;
		};

	public:
		float X;
		float Y;
		float RelX;
		float RelY;
		float XScroll;
		float YScroll;
		float RelXScroll;
		float RelYScroll;
		bool IsVisible;
		ButtonState Left;
		ButtonState Middle;
		ButtonState Right;
		Button Buttons[BLT_MAX_MOUSE_BUTTONS];

	};

	struct BLT_API KeyboardInstance
	{
	public:
		struct BLT_API Key
		{
		public:
			Keycode KeyID;
			ButtonState State;
		};

	public:
		Key Keys[BLT_MAX_KEYS];

	};

	class BLT_API Input
	{
	private:
		static MouseInstance s_Mouse;
		static KeyboardInstance s_Keyboard;
		static Window* s_Window;

		static std::vector<char> s_PressedCharacters;
		static std::vector<KeyboardInstance::Key> s_ChangedKeys;
		static std::vector<MouseInstance::Button> s_ChangedButtons;

	public:
		Input() = delete;

		static const MouseInstance& Mouse();
		static const KeyboardInstance& Keyboard();

		static const std::vector<char>& PressedCharacters();

		// Mouse position relative to the bottom left of the window
		static Vector3f MousePosition();
		// Mouse position change since last frame relative to the bottom left of the window
		static Vector3f RelMousePosition();
		// Mouse position in the range [-1, 1] from the centre of the window
		static Vector3f NormalizedMousePosition();
		// Mouse posiiton change from last frame in the range [-1, 1] from the centre of the window
		static Vector3f RelNormalizedMousePosition();
		// Total scroll vector of the mouse
		static Vector2f MouseScroll();
		// Change in scroll from the last frame
		static Vector2f RelMouseScroll();

		// Returns true if mouseButton is held down during this frame
		static bool MouseButtonDown(MouseButton mouseButton, KeyMod mods = KeyMod::Ignore);
		// Returns true if mouseButton was pressed this frame
		static bool MouseButtonPressed(MouseButton mouseButton, KeyMod mods = KeyMod::Ignore);
		// Returns true if mouseButton is not held down during this frame
		static bool MouseButtonUp(MouseButton mouseButton, KeyMod mods = KeyMod::Ignore);
		// Returns true if mouseButton was released this frame
		static bool MouseButtonReleased(MouseButton mouseButton, KeyMod mods = KeyMod::Ignore);

		// Returns true if key is held down during this frame
		static bool KeyDown(Keycode key, KeyMod mods = KeyMod::Ignore);
		// Returns true if key is pressed this frame
		static bool KeyPressed(Keycode key, KeyMod mods = KeyMod::Ignore);
		// Returns true if key is not held down during this frame
		static bool KeyUp(Keycode key, KeyMod mods = KeyMod::Ignore);
		// Returns true if key is released this frame
		static bool KeyReleased(Keycode key, KeyMod mods = KeyMod::Ignore);

		static void HideCursor();
		static void ShowCursor();

		friend class Initializer;
		friend class Window;
		friend class Engine;

	private:
		static void Update();
		static void Initialize(Window* window);
		static void Terminate();
		static KeyboardInstance::Key* GetKey(Keycode keycode);
		static MouseInstance::Button* GetMouseButton(MouseButton button);
		static bool TestKeyMods(KeyMod mods);

		static void MousePositionCallback(GLFWwindow* window, double mouseX, double mouseY);
		static void MouseScrollCallback(GLFWwindow* window, double relX, double relY);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void KeyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void WindowResizeCallback(GLFWwindow* window, int width, int height);
		static void CharPressedCallback(GLFWwindow* window, uint code);

	};

}