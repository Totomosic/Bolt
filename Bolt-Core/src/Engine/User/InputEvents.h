#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	BLT_API enum class MouseButton
	{
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,
		Right = GLFW_MOUSE_BUTTON_RIGHT
	};

	BLT_API enum class ButtonState
	{
		Released = GLFW_RELEASE,
		Pressed = GLFW_PRESS
	};

	BLT_API enum class Keycode
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

	BLT_API enum class KeyMod
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

	struct BLT_API KeyPressedEvent : public Event
	{
	public:
		Keycode KeyCode;
		bool IsRepeat;
	};

	struct BLT_API KeyReleasedEvent : public Event
	{
	public:
		Keycode KeyCode;
	};

	struct BLT_API MouseMovedEvent : public Event
	{
	public:
		float x;
		float y;
		float relX;
		float relY;
	};

	struct BLT_API MousePressedEvent : public Event
	{
	public:
		MouseButton Button;
		bool IsRepeat;
		float x;
		float y;
	};

	struct BLT_API MouseReleasedEvent : public Event
	{
	public:
		MouseButton Button;
		float x;
		float y;
	};

	struct BLT_API MouseScrolledEvent : public Event
	{
	public:
		float xOffset;
		float yOffset;
	};

}