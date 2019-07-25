#include "bltpch.h"
#include "InputMemoryStream.h"

namespace Bolt
{

	InputMemoryStream::InputMemoryStream() : InputMemoryStream(1)
	{
		
	}

	InputMemoryStream::InputMemoryStream(uint32_t capacity)
		: m_Buffer(std::make_unique<byte[]>(capacity)), m_Capacity(capacity), m_Head(0)
	{
		
	}

	byte* InputMemoryStream::GetBufferPtr() const
	{
		return m_Buffer.get() + m_Head;
	}

	int InputMemoryStream::GetRemainingDataSize() const
	{
		return m_Capacity - m_Head;
	}

	void InputMemoryStream::Reset(int to)
	{
		m_Head = to;
	}

	void InputMemoryStream::Read(void* buffer, uint32_t length)
	{
		BLT_ASSERT(length <= GetRemainingDataSize(), "Not enough data left in Input Stream");
		memcpy(buffer, GetBufferPtr(), length);
		m_Head += length;
	}

}