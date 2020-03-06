#pragma once
#include "MemoryStream.h"

namespace Bolt
{

	class InputMemoryStream;

	class BLT_API OutputMemoryStream : public MemoryStream
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
		void WriteFromStream(InputMemoryStream& stream, size_t length);

		template<typename T>
		void Write(const T& value)
		{
			Write(&value, sizeof(T));
		}

		OutputMemoryStream Clone() const;

	private:
		void ReallocBuffer(size_t capacity);
		void TestRealloc(size_t length);
		void* GetHeadPtr();
	};

}