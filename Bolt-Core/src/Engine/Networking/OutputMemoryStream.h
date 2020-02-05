#pragma once
#include "bltpch.h"

namespace Bolt
{

	class BLT_API OutputMemoryStream
	{
	private:
		std::unique_ptr<byte[]> m_Buffer;
		size_t m_Capacity;
		size_t m_Head;

	public:
		OutputMemoryStream(size_t capacity = 1);

		void* GetBufferPtr() const;
		size_t GetRemainingDataSize() const;
		void Reset(size_t to = 0);

		void Write(const void* data, size_t length);

		template<typename T>
		void Write(const T& value)
		{
			Write(&value, sizeof(T));
		}

		void Skip(size_t nbytes);

	private:
		void ReallocBuffer(size_t capacity);

	};

}