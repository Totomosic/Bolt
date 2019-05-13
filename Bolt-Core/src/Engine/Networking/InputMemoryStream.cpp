#include "Types.h"
#include "InputMemoryStream.h"

namespace Bolt
{

	InputMemoryStream::InputMemoryStream() : InputMemoryStream(1)
	{
		
	}

	InputMemoryStream::InputMemoryStream(uint32_t capacity)
		: m_Buffer(new byte[capacity]), m_Capacity(capacity), m_Head(0)
	{
		
	}

	InputMemoryStream::InputMemoryStream(InputMemoryStream&& other)
		: m_Buffer(other.m_Buffer), m_Capacity(other.m_Capacity), m_Head(other.m_Head)
	{
		other.m_Buffer = nullptr;
	}

	InputMemoryStream& InputMemoryStream::operator=(InputMemoryStream&& other)
	{
		byte* buff = m_Buffer;
		m_Buffer = other.m_Buffer;
		other.m_Buffer = buff;
		m_Capacity = other.m_Capacity;
		m_Head = other.m_Head;
		return *this;
	}

	InputMemoryStream::~InputMemoryStream()
	{
		if (m_Buffer != nullptr)
		{
			delete[] m_Buffer;
		}
	}

	byte* InputMemoryStream::GetBufferPtr() const
	{
		return m_Buffer + m_Head;
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