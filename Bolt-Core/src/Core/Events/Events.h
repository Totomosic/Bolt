#pragma once
#include "EventManager.h"

namespace Bolt
{

	struct BLT_API Events
	{
	public:
		// Input Events
		static constexpr id_t KEY_PRESSED = 0;
		static constexpr id_t KEY_RELEASED = 1;
		static constexpr id_t MOUSE_PRESSED = 2;
		static constexpr id_t MOUSE_RELEASED = 3;

		// UI Events
		static constexpr id_t ON_HOVER_ENTRY = 4;
		static constexpr id_t ON_HOVER = 5;
		static constexpr id_t ON_HOVER_EXIT = 6;
		static constexpr id_t ON_CLICKED = 7;

	};

}