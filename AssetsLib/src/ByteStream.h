#pragma once
#include "Logging.h"

namespace Bolt::Assets
{

	class BLT_API ByteStream
	{
	private:
		size_t m_Length;
		std::unique_ptr<uint8_t[]> m_Data;

	public:
		ByteStream(size_t length);

		size_t GetLength() const;
		const void* GetData() const;
		void* GetData();
	};

}