#include "Types.h"
#include "InputMemoryStream.h"

namespace Bolt
{

	InputMemoryStream::InputMemoryStream(uint capacity)
		: m_Buffer(new byte[capacity]), m_Capacity(capacity), m_Head(0)
	{
		
	}

	InputMemoryStream::~InputMemoryStream()
	{
		delete[] m_Buffer;
	}

	byte* InputMemoryStream::GetBufferPtr() const
	{
		return m_Buffer;
	}

	int InputMemoryStream::GetRemainingDataSize() const
	{
		return m_Capacity - m_Head;
	}

	void InputMemoryStream::Read(void* buffer, uint length)
	{
		BLT_ASSERT(length <= GetRemainingDataSize(), "Not enough data left in Input Stream");
		memcpy(buffer, GetBufferPtr() + m_Head, length);
		m_Head += length;
	}

}