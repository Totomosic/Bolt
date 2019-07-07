#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace Aimbooster
{

	constexpr int TARGET_HIT_EVENT = 10000;
	constexpr int TARGET_FAILED_EVENT = 10001;

	class TargetFailedEvent
	{
	public:
		Vector3f Position;
	};

}