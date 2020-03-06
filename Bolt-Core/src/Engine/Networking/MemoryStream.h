#pragma once
#include "bltpch.h"

namespace Bolt
{

	class BLT_API MemoryStream
	{
	public:
		MemoryStream();
		virtual ~MemoryStream() {}

	protected:
		int CalculateNBits(size_t range) const;
		int CalculateNBytes(size_t nbits) const;

		template<typename T>
		T NormalizeValue(T value, T min) const
		{
			return value - min;
		}

		template<typename T>
		T DenormalizeValue(T value, T min) const
		{
			return value + min;
		}

		template<typename T>
		bool CheckRange(T value, T min, T max) const
		{
			return min < max && value >= min && value <= max;
		}
	};

}