#include "ByteStream.h"

namespace Bolt::Assets
{

	ByteStream::ByteStream(size_t length)
		: m_Length(length), m_Data(std::make_unique<uint8_t[]>(length))
	{
	}

	size_t ByteStream::GetLength() const
	{
		return m_Length;
	}

	const void* ByteStream::GetData() const
	{
		return (const void*)m_Data.get();
	}

	void* ByteStream::GetData()
	{
		return (void*)m_Data.get();
	}

}
