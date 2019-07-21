#include "bltpch.h"
#include "OutputMemoryStream.h"

namespace Bolt
{

	OutputMemoryStream::OutputMemoryStream(uint32_t capacity)
		: m_Buffer(std::make_unique<byte[]>(capacity)), m_Capacity(capacity), m_Head(0)
	{
		
	}

	byte* OutputMemoryStream::GetBufferPtr() const
	{
		return m_Buffer.get();
	}

	int OutputMemoryStream::GetRemainingDataSize() const
	{
		return m_Head;
	}

	void OutputMemoryStream::Reset(int to)
	{
		m_Head = to;
	}

	void OutputMemoryStream::Write(const void* data, uint32_t length)
	{
		if (m_Capacity < m_Head + length)
		{
			ReallocBuffer(std::max(m_Capacity * 2, m_Head + (int)length));
		}
		memcpy(GetBufferPtr() + m_Head, data, length);
		m_Head += length;
	}

	void OutputMemoryStream::ReallocBuffer(uint32_t capacity)
	{
		byte* data = new byte[GetRemainingDataSize()];
		memcpy(data, GetBufferPtr(), GetRemainingDataSize());
		m_Buffer = std::make_unique<byte[]>(capacity);
		memcpy(m_Buffer.get(), data, GetRemainingDataSize());
		m_Capacity = capacity;
		delete[] data;
	}

}