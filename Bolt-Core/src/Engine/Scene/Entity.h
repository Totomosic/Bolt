#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct BLT_API Entity
	{
	public:
		using Id_t = id_t;
		static constexpr Id_t InvalidId = std::numeric_limits<Id_t>::max();

	public:
		id_t Id;
	};

}