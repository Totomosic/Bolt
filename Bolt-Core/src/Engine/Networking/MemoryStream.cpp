#include "bltpch.h"
#include "MemoryStream.h"

namespace Bolt
{

	MemoryStream::MemoryStream()
	{
	}

	int MemoryStream::CalculateNBits(size_t range) const
	{
		size_t divisor = log2(range);
		return range / divisor + (range % divisor == 0 ? 0 : 1);
	}

	int MemoryStream::CalculateNBytes(size_t nbits) const
	{
		static constexpr int8_t BITS_PER_BYTE = 8;
		return nbits / BITS_PER_BYTE + (nbits % BITS_PER_BYTE == 0 ? 0 : 1);
	}

}
