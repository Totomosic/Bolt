#pragma once
#include "MemoryStream.h"
#include "OutputMemoryStream.h"

namespace Bolt
{

	class BLT_API InputMemoryStream : public MemoryStream
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

		InputMemoryStream Clone() const;

	public:
		static InputMemoryStream FromStream(const OutputMemoryStream& stream);
	};

}