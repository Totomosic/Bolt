#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	class BLT_API OutputMemoryStream
	{
	private:
		byte* m_Buffer;
		int m_Capacity;
		int m_Head;

	public:
		OutputMemoryStream(uint capacity = 1);
		OutputMemoryStream(const OutputMemoryStream& other) = delete;
		OutputMemoryStream& operator=(const OutputMemoryStream& other) = delete;
		OutputMemoryStream(OutputMemoryStream&& other);
		OutputMemoryStream& operator=(OutputMemoryStream&& other);
		~OutputMemoryStream();

		byte* GetBufferPtr() const;
		int GetRemainingDataSize() const;
		void Reset(int to = 0);

		void Write(const void* data, uint length);

		template<typename T>
		void Write(const T& value)
		{
			Write(&value, sizeof(T));
		}

	private:
		void ReallocBuffer(uint capacity);

	};

}