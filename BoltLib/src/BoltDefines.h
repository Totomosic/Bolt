#pragma once
#include "Logging.h"

namespace Bolt
{

	using byte = uint8_t;
	using id_t = uint32_t;

}

#define BLT_OFFSET_OF(object, field) ((uint32_t)(&(((object*)(nullptr))->field)))
#define BLT_BIT(x) (1 << (x))
#define BLT_IS_BIT_SET(var, num) (!!((var) & BLT_BIT(num)))
#define BLT_CONTAINS_BIT(var, bitvalue) (!!(((int)var) & ((int)bitvalue)))

// Event Helpers
#define BLT_BIND_EVENT_FN(func) std::bind(&func, this, std::placeholders::_1)