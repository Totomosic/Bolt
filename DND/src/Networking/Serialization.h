#pragma once
#include "bltpch.h"

namespace DND
{

#define DND_CREATE_SERIALIZE_SIMPLE_TYPE(Type)	\
	template<>	\
	inline void Serialize(OutputMemoryStream& stream, const Type& value)	\
	{	\
		stream.Write(value);	\
	}	\

	template<typename T>
	inline void Serialize(OutputMemoryStream& stream, const T& value)
	{
		BLT_ASSERT(false, "Unable to serialize type " + typeid(T).name());
	}

	DND_CREATE_SERIALIZE_SIMPLE_TYPE(char);
	DND_CREATE_SERIALIZE_SIMPLE_TYPE(byte);
	DND_CREATE_SERIALIZE_SIMPLE_TYPE(int);
	DND_CREATE_SERIALIZE_SIMPLE_TYPE(id_t);
	DND_CREATE_SERIALIZE_SIMPLE_TYPE(float);
	DND_CREATE_SERIALIZE_SIMPLE_TYPE(double);

	DND_CREATE_SERIALIZE_SIMPLE_TYPE(SocketAddress);

	template<>
	inline void Serialize(OutputMemoryStream& stream, const Vector2f& value)
	{
		stream.Write(value.x);
		stream.Write(value.y);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const Vector3f& value)
	{
		stream.Write(value.x);
		stream.Write(value.y);
		stream.Write(value.z);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const Vector2i& value)
	{
		stream.Write(value.x);
		stream.Write(value.y);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const Vector3i& value)
	{
		stream.Write(value.x);
		stream.Write(value.y);
		stream.Write(value.z);
	}

	template<typename T>
	inline void Serialize(OutputMemoryStream& stream, const std::vector<T>& value)
	{
		Serialize(stream, value.size());
		for (const T& v : value)
		{
			Serialize(stream, v);
		}
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const blt::string& value)
	{
		Serialize(stream, value.size());
		stream.Write(value.data(), value.size());
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const OutputMemoryStream& value)
	{
		Serialize(stream, value.GetRemainingDataSize());
		stream.Write(value.GetBufferPtr(), value.GetRemainingDataSize());
	}

}