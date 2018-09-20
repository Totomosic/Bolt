#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	class BLT_API Resource
	{
	public:
		inline virtual ~Resource() {}

		virtual Resource* Clone() const = 0;

	};

}