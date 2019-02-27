#pragma once
#include "Types.h"

namespace Bolt
{

	struct BLT_API Events
	{
	public:
		static constexpr id_t NONE							= 0;
		// Input Events
		static constexpr id_t KEY_PRESSED					= 1;
		static constexpr id_t KEY_RELEASED					= 2;
		static constexpr id_t MOUSE_PRESSED					= 3;
		static constexpr id_t MOUSE_RELEASED				= 4;
		static constexpr id_t MOUSE_SCROLLED				= 5;
		static constexpr id_t MOUSE_POSITION_MOVED			= 6;

		// UI Events
		static constexpr id_t ON_HOVER_ENTRY				= 7;
		static constexpr id_t ON_HOVER						= 8;
		static constexpr id_t ON_HOVER_EXIT					= 9;
		static constexpr id_t ON_CLICKED					= 10;

		// Window Events
		static constexpr id_t WINDOW_RESIZE					= 11;
		static constexpr id_t WINDOW_MOVED					= 12;
		static constexpr id_t WINDOW_FOCUSED				= 13;
		static constexpr id_t WINDOW_LOST_FOCUS				= 14;
		static constexpr id_t WINDOW_CLOSED					= 15;

		// Scene Events
		static constexpr id_t SCENE_LOADED					= 16;
		static constexpr id_t SCENE_UNLOADED				= 17;
		static constexpr id_t GAMEOBJECT_INSTANTIATED		= 18;
		static constexpr id_t GAMEOBJECT_DESTROYED			= 19;

		// Debugging Events
		static constexpr id_t COMMAND_LINE_MESSAGE_RECEIVED	= 20;

		// Task Events
		static constexpr id_t TASK_CONTINUE_ON_MAIN_THREAD	= 21;

	};

}