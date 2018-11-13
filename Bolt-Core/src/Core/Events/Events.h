#pragma once
#include "EventManager.h"

namespace Bolt
{

	struct BLT_API Events
	{
	public:
		static constexpr id_t KEY_PRESSED = 0;
		static constexpr id_t KEY_RELEASED = 1;
		static constexpr id_t MOUSE_PRESSED = 2;
		static constexpr id_t MOUSE_RELEASED = 3;

	};

}