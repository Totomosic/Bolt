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
		MouseInstance s_Mouse;
		KeyboardInstance s_Keyboard;
		Window* s_Window;

		std::vector<char> s_PressedCharacters;
		std::vector<KeyboardInstance::Key> s_ChangedKeys;
		std::vector<MouseInstance::Button> s_ChangedButtons;

	public:
		EventDispatcher<Bolt::KeyPressedEvent> OnKeyPressed;
		EventDispatcher<Bolt::KeyReleasedEvent> OnKeyReleased;
		EventDispatcher<Bolt::MousePressedEvent> OnMousePressed;
		EventDispatcher<Bolt::MouseReleasedEvent> OnMouseReleased;
		EventDispatcher<Bolt::MouseScrolledEvent> OnMouseScrolled;
		EventDispatcher<Bolt::MouseMovedEvent> OnMouseMoved;

	private:
		Input();

	public:
		static Input& Get();

	public:
		const MouseInstance& Mouse();
		const KeyboardInstance& Keyboard();

		const std::vector<char>& PressedCharacters();

		// Mouse position relative to the bottom left of the window
		Vector3f MousePosition();
		// Mouse position relative to bottom left of region with given [width, height]
		Vector3f MousePosition(float width, float height);
		// Mouse position change since last frame relative to the bottom left of the window
		Vector3f RelMousePosition();
		// Mouse position change since last frame relative to the bottom left of region with given [width, height]
		Vector3f RelMousePosition(float width, float height);
		// Mouse position in the range [-1, 1] from the centre of the window
		Vector3f NormalizedMousePosition();
		// Mouse posiiton change from last frame in the range [-1, 1] from the centre of the window
		Vector3f RelNormalizedMousePosition();
		// Total scroll vector of the mouse
		Vector2f MouseScroll();
		// Change in scroll from the last frame
		Vector2f RelMouseScroll();

		// Returns true if mouseButton is held down during this frame
		bool MouseButtonDown(MouseButton mouseButton, KeyMod mods = KeyMod::Ignore);
		// Returns true if mouseButton was pressed this frame
		bool MouseButtonPressed(MouseButton mouseButton, KeyMod mods = KeyMod::Ignore);
		// Returns true if mouseButton is not held down during this frame
		bool MouseButtonUp(MouseButton mouseButton, KeyMod mods = KeyMod::Ignore);
		// Returns true if mouseButton was released this frame
		bool MouseButtonReleased(MouseButton mouseButton, KeyMod mods = KeyMod::Ignore);

		// Returns true if key is held down during this frame
		bool KeyDown(Keycode key, KeyMod mods = KeyMod::Ignore);
		// Returns true if key is pressed this frame
		bool KeyPressed(Keycode key, KeyMod mods = KeyMod::Ignore);
		// Returns true if key is not held down during this frame
		bool KeyUp(Keycode key, KeyMod mods = KeyMod::Ignore);
		// Returns true if key is released this frame
		bool KeyReleased(Keycode key, KeyMod mods = KeyMod::Ignore);

		void HideCursor();
		void ShowCursor();

		friend class Initializer;
		friend class Destructor;
		friend class Window;
		friend class Engine;

	private:
		void Update();
		void Initialize(Window* window);
		void Terminate();
		KeyboardInstance::Key* GetKey(Keycode keycode);
		MouseInstance::Button* GetMouseButton(MouseButton button);
		bool TestKeyMods(KeyMod mods);

		void MousePositionCallback(double mouseX, double mouseY);
		void MouseScrollCallback(double relX, double relY);
		void MouseButtonCallback(int button, int action, int mods);
		void KeyboardKeyCallback(int key, int scancode, int action, int mods);
		void CharPressedCallback(uint code);

	};

}