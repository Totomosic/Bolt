#pragma once
#include "bltpch.h"

namespace DND
{

#define DND_CREATE_DESERIALIZE_SIMPLE_TYPE(Type)	\
	template<>	\
	inline void Deserialize(InputMemoryStream& stream, Type& outValue)	\
	{	\
		stream.Read(&outValue);	\
	}	\

	template<typename T>
	inline void Deserialize(InputMemoryStream& stream, T& outValue)
	{
		BLT_ASSERT(false, "Unable to deserialize type " + typeid(T).name());
	}

	DND_CREATE_DESERIALIZE_SIMPLE_TYPE(char);
	DND_CREATE_DESERIALIZE_SIMPLE_TYPE(byte);
	DND_CREATE_DESERIALIZE_SIMPLE_TYPE(int);
	DND_CREATE_DESERIALIZE_SIMPLE_TYPE(id_t);
	DND_CREATE_DESERIALIZE_SIMPLE_TYPE(float);
	DND_CREATE_DESERIALIZE_SIMPLE_TYPE(double);

	DND_CREATE_DESERIALIZE_SIMPLE_TYPE(SocketAddress);

	template<>
	inline void Deserialize(InputMemoryStream& stream, Vector2f& value)
	{
		stream.Read(&value.x);
		stream.Read(&value.y);
	}

	template<>
	inline void Deserialize(InputMemoryStream& stream, Vector3f& value)
	{
		stream.Read(&value.x);
		stream.Read(&value.y);
		stream.Read(&value.z);
	}

	template<>
	inline void Deserialize(InputMemoryStream& stream, Vector2i& value)
	{
		stream.Read(&value.x);
		stream.Read(&value.y);
	}

	template<>
	inline void Deserialize(InputMemoryStream& stream, Vector3i& value)
	{
		stream.Read(&value.x);
		stream.Read(&value.y);
		stream.Read(&value.z);
	}

	template<typename T>
	inline void Deserialize(InputMemoryStream& stream, std::vector<T>& outValue)
	{
		size_t length;
		Deserialize(stream, length);
		outValue.resize(length);
		for (size_t i = 0; i < length; i++)
		{
			Deserialize(stream, outValue[i]);
		}
	}

	template<>
	inline void Deserialize(InputMemoryStream& stream, blt::string& outValue)
	{
		size_t length;
		Deserialize(stream, length);
		char* buffer = new char[length];
		stream.Read(buffer, length);
		outValue = blt::string(buffer, length);
		delete[] buffer;
	}

	template<>
	inline void Deserialize(InputMemoryStream& stream, OutputMemoryStream& outValue)
	{
		size_t length;
		Deserialize(stream, length);
		OutputMemoryStream outStream(length);
		byte* buffer = new byte[length];
		stream.Read(buffer, length);
		outStream.Write(buffer, length);
		outValue = std::move(outStream);
		delete[] buffer;
	}

}