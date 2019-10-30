#pragma once
#include "bltpch.h"

namespace Bolt
{

	class BLT_API Resource
	{
	public:
		inline virtual ~Resource() {}

		virtual std::unique_ptr<Resource> Clone() const = 0;

	};

}