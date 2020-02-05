#include "bltpch.h"
#include "OutputMemoryStream.h"

namespace Bolt
{

	OutputMemoryStream::OutputMemoryStream(size_t capacity)
		: m_Buffer(std::make_unique<byte[]>(capacity)), m_Capacity(capacity), m_Head(0)
	{
		
	}

	void* OutputMemoryStream::GetBufferPtr() const
	{
		return m_Buffer.get();
	}

	size_t OutputMemoryStream::GetRemainingDataSize() const
	{
		return m_Head;
	}

	void OutputMemoryStream::Reset(size_t to)
	{
		m_Head = to;
	}

	void OutputMemoryStream::Write(const void* data, size_t length)
	{
		if (m_Capacity < m_Head + length)
		{
			ReallocBuffer(std::max(m_Capacity * 2, m_Head + length));
		}
		memcpy((char*)GetBufferPtr() + m_Head, data, length);
		m_Head += length;
	}

	void OutputMemoryStream::Skip(size_t nbytes)
	{
		if (m_Capacity < m_Head + nbytes)
		{
			ReallocBuffer(std::max(m_Capacity * 2, m_Head + nbytes));
		}
		m_Head += nbytes;
	}

	void OutputMemoryStream::ReallocBuffer(size_t capacity)
	{
		byte* data = BLT_NEW byte[GetRemainingDataSize()];
		memcpy(data, GetBufferPtr(), GetRemainingDataSize());
		m_Buffer = std::make_unique<byte[]>(capacity);
		memcpy(m_Buffer.get(), data, GetRemainingDataSize());
		m_Capacity = capacity;
		BLT_DELETE_ARR data;
	}

}