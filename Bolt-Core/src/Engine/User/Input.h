#pragma once
#include "Bolt-Graphics.h"
#include "InputEvents.h"

#define BLT_MAX_KEYS 512
#define BLT_MAX_MOUSE_BUTTONS 32

namespace Bolt
{

	struct BLT_API MouseInstance
	{
	public:
		struct BLT_API Button
		{
			int ButtonID;
			int State;
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
			int KeyID;
			int State;
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
		static EventDispatcher<Bolt::KeyPressedEvent> OnKeyPressed;
		static EventDispatcher<Bolt::KeyReleasedEvent> OnKeyReleased;
		static EventDispatcher<Bolt::MousePressedEvent> OnMousePressed;
		static EventDispatcher<Bolt::MouseReleasedEvent> OnMouseReleased;
		static EventDispatcher<Bolt::MouseScrolledEvent> OnMouseScrolled;
		static EventDispatcher<Bolt::MouseMovedEvent> OnMouseMoved;

	public:
		Input() = delete;

		static const MouseInstance& Mouse();
		static const KeyboardInstance& Keyboard();

		static const std::vector<char>& PressedCharacters();

		// Mouse position relative to the bottom left of the window
		static Vector3f MousePosition();
		// Mouse position relative to bottom left of region with given [width, height]
		static Vector3f MousePosition(float width, float height);
		// Mouse position change since last frame relative to the bottom left of the window
		static Vector3f RelMousePosition();
		// Mouse position change since last frame relative to the bottom left of region with given [width, height]
		static Vector3f RelMousePosition(float width, float height);
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
		friend class Destructor;
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
		static void WindowClosedCallback(GLFWwindow* window);

	};

}