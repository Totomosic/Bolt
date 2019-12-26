#pragma once
#include "BoltDefines.h"

namespace Bolt
{

	template<typename T>
	class BLT_API GenericTimeDelta
	{
	private:
		T m_Seconds;

	public:
		GenericTimeDelta() : GenericTimeDelta((T)0)
		{
		
		}

		GenericTimeDelta(T seconds)
			: m_Seconds(seconds)
		{
		
		}

		inline T Seconds() const { return m_Seconds; }
		inline T Milliseconds() const { return Seconds() * 1000; }
		inline T Microseconds() const { return Milliseconds() * 1000; }
		inline T Nanoseconds() const { return Microseconds() * 1000; }

	};

	using TimeDelta = ::Bolt::GenericTimeDelta<double>;

}