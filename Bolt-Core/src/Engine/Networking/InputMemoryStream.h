#pragma once
#include "bltpch.h"

namespace Bolt
{

	class BLT_API InputMemoryStream
	{
	private:
		std::unique_ptr<byte[]> m_Buffer;
		size_t m_Capacity;
		size_t m_Head;

	public:
		InputMemoryStream();
		InputMemoryStream(size_t capacity);

		void* GetBufferPtr() const;
		int GetRemainingDataSize() const;
		void Reset(size_t to = 0);

		void Read(void* buffer, size_t length);
		
		template<typename T>
		void Read(T* outValue)
		{
			Read((void*)outValue, sizeof(T));
		}

		void Skip(size_t nbytes);

	};

}