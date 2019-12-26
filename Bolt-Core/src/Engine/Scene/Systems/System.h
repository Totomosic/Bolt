#pragma once
#include "../EntityManager.h"
#include "Core/Time/TimeDelta.h"

namespace Bolt
{

	class BLT_API BaseSystem
	{
	public:
		using Family = size_t;

	protected:
		static Family s_FamilyCounter;

	public:
		virtual ~BaseSystem() {}
		// Called once on startup when all systems have been added
		virtual void Configure() {}
		virtual void Update(EntityManager& manager, TimeDelta delta) = 0;
	};

	template<typename T>
	class BLT_API System : public BaseSystem
	{
	public:
		static Family GetFamily()
		{
			static Family family = s_FamilyCounter++;
			return family;
		}		
	};

}