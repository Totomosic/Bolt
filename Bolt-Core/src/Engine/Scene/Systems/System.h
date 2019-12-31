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

	private:
		bool m_IsEnabled;

	public:
		BaseSystem();
		virtual ~BaseSystem() {}

		bool IsEnabled() const;
		void SetEnabled(bool enabled);
		inline void Enable() { SetEnabled(true); }
		inline void Disable() { SetEnabled(false); }

		// Called once on startup when all systems have been added
		virtual void Configure() {}
		virtual void Update(EntityManager& manager, TimeDelta delta) = 0;

	protected:
		virtual void SetEnabledInternal(bool enabled);
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