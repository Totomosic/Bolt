#include "Types.h"
#include "OutputMemoryStream.h"

namespace Bolt
{

	OutputMemoryStream::OutputMemoryStream(uint capacity)
		: m_Buffer(nullptr), m_Capacity(capacity), m_Head(0)
	{
		ReallocBuffer(m_Capacity);
	}

	OutputMemoryStream::~OutputMemoryStream()
	{
		if (m_Buffer != nullptr)
		{
			std::free(m_Buffer);
		}
	}

	byte* OutputMemoryStream::GetBufferPtr() const
	{
		return m_Buffer;
	}

	int OutputMemoryStream::GetRemainingDataSize() const
	{
		return m_Head;
	}

	void OutputMemoryStream::Write(const void* data, uint length)
	{
		if (m_Capacity < m_Head + length)
		{
			ReallocBuffer(std::max(m_Capacity * 2, m_Head + (int)length));
		}
		memcpy(m_Buffer + m_Head, data, length);
		m_Head += length;
	}

	void OutputMemoryStream::ReallocBuffer(uint capacity)
	{
		m_Buffer = (byte*)std::realloc(m_Buffer, capacity);
	}

}