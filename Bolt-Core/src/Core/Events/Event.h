#pragma once
#include "Types.h"
#include "Events.h"

namespace Bolt
{

#define BLT_EVENT_ID EventId
#define BLT_EVENT_ID_DEF(Id)	\
	constexpr static id_t BLT_EVENT_ID = Id

	class BLT_API Event
	{
	public:
		inline virtual ~Event() {}
	};

}