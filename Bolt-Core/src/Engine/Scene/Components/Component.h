#pragma once
#include "bltpch.h"

namespace Bolt
{

	class BLT_API BaseComponent
	{
	public:
		using Family = size_t;

	protected:
		static Family s_FamilyCounter;
	};

	template<typename T>
	class BLT_API Component : public BaseComponent
	{
	public:
		static Family GetFamily()
		{
			static Family family = s_FamilyCounter++;
			return family;
		}
	};

}