#pragma once
#include "bltpch.h"

namespace Bolt
{

	class BLT_API OutputMemoryStream
	{
	private:
		std::unique_ptr<byte[]> m_Buffer;
		int m_Capacity;
		int m_Head;

	public:
		OutputMemoryStream(uint32_t capacity = 1);

		byte* GetBufferPtr() const;
		int GetRemainingDataSize() const;
		void Reset(int to = 0);

		void Write(const void* data, uint32_t length);

		template<typename T>
		void Write(const T& value)
		{
			Write(&value, sizeof(T));
		}

	private:
		void ReallocBuffer(uint32_t capacity);

	};

}