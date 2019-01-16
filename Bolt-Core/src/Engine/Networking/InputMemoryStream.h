#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	class BLT_API InputMemoryStream
	{
	private:
		byte* m_Buffer;
		int m_Capacity;
		int m_Head;

	public:
		InputMemoryStream();
		InputMemoryStream(uint capacity);
		InputMemoryStream(const InputMemoryStream& other) = delete;
		InputMemoryStream& operator=(const InputMemoryStream& other) = delete;
		InputMemoryStream(InputMemoryStream&& other);
		InputMemoryStream& operator=(InputMemoryStream&& other);
		~InputMemoryStream();

		byte* GetBufferPtr() const;
		int GetRemainingDataSize() const;

		void Read(void* buffer, uint length);
		
		template<typename T>
		void Read(T* outValue)
		{
			Read((void*)outValue, sizeof(T));
		}

	};

}