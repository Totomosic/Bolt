#pragma once
#include "bltpch.h"
#include "Core/Math/Maths.h"
#include "Core/Color.h"
#include "../BufferLayout.h"

namespace Bolt
{

	class BLT_API AttributeSetter
	{
	private:
		void* m_Ptr;
		int m_AttribIndex;

	public:
		AttributeSetter(void* ptr, int attribIndex);

		const void* Get() const;
		void* Get();
		int AttributeIndex() const;

		// When converting to T&, use explicit_cast<>
		template<typename T>
		explicit operator T()
		{
			return Read<std::remove_reference<T>::type>();
		}

		template<typename T>
		const T& Read() const
		{
			BLT_ASSERT(false, "Unable to convert tyype")
			return *(T*)nullptr;
		}

		template<typename T>
		T& Read()
		{
			BLT_ASSERT(false, "Unable to convert to type");
			return *(T*)nullptr;
		}

		template<typename T>
		AttributeSetter& operator=(const T& obj)
		{
			Set(obj);
			return *this;
		}

		template<typename T>
		void Set(const T& obj)
		{
			BLT_ASSERT(false, "Type unsupported for vertex modification");
		}

		template<typename T>
		AttributeSetter& operator+=(const T& obj)
		{
			Set(Read<T>() + obj);
			return *this;
		}

		template<typename T>
		AttributeSetter& operator-=(const T& obj)
		{
			Read<T>() -= obj;
			return *this;
		}

		template<typename T>
		AttributeSetter& operator*=(const T& obj)
		{
			Read<T>() *= obj;
			return *this;
		}

		template<typename T>
		AttributeSetter& operator/=(const T& obj)
		{
			Read<T>() /= obj;
			return *this;
		}

	private:
		bool ValidateAttribute(int attribIndex, DataType type, int count) const;

	};

	template<>
	inline const Vector2f& AttributeSetter::Read<Vector2f>() const
	{
		return *(Vector2f*)m_Ptr;
	}

	template<>
	inline const Vector3f& AttributeSetter::Read<Vector3f>() const
	{
		return *(Vector3f*)m_Ptr;
	}

	template<>
	inline const Vector4f& AttributeSetter::Read<Vector4f>() const
	{
		return *(Vector4f*)m_Ptr;
	}

	template<>
	inline const float& AttributeSetter::Read<float>() const
	{
		return *(float*)m_Ptr;
	}

	template<>
	inline const int& AttributeSetter::Read<int>() const
	{
		return *(int*)m_Ptr;
	}

	template<>
	inline Vector2f& AttributeSetter::Read<Vector2f>()
	{
		return *(Vector2f*)m_Ptr;
	}

	template<>
	inline Vector3f& AttributeSetter::Read<Vector3f>()
	{
		return *(Vector3f*)m_Ptr;
	}

	template<>
	inline Vector4f& AttributeSetter::Read<Vector4f>()
	{
		return *(Vector4f*)m_Ptr;
	}

	template<>
	inline Color& AttributeSetter::Read<Color>()
	{
		return *(Color*)m_Ptr;
	}

	template<>
	inline float& AttributeSetter::Read<float>()
	{
		return *(float*)m_Ptr;
	}

	template<>
	inline byte& AttributeSetter::Read<byte>()
	{
		return *(byte*)m_Ptr;
	}

	template<>
	inline Vector2<byte>& AttributeSetter::Read<Vector2<byte>>()
	{
		return *(Vector2<byte>*)m_Ptr;
	}

	template<>
	inline Vector3<byte>& AttributeSetter::Read<Vector3<byte>>()
	{
		return *(Vector3<byte>*)m_Ptr;
	}

	template<>
	inline Vector4<byte>& AttributeSetter::Read<Vector4<byte>>()
	{
		return *(Vector4<byte>*)m_Ptr;
	}

	template<>
	inline Vector2i& AttributeSetter::Read<Vector2i>()
	{
		return *(Vector2i*)m_Ptr;
	}

	template<>
	inline Vector3i& AttributeSetter::Read<Vector3i>()
	{
		return *(Vector3i*)m_Ptr;
	}

	template<>
	inline Vector4i& AttributeSetter::Read<Vector4i>()
	{
		return *(Vector4i*)m_Ptr;
	}

	template<>
	inline int& AttributeSetter::Read<int>()
	{
		return *(int*)m_Ptr;
	}

	template<>
	inline unsigned int& AttributeSetter::Read<unsigned int>()
	{
		return *(unsigned int*)m_Ptr;
	}

	template<>
	inline void AttributeSetter::Set<Vector2f>(const Vector2f& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::Float, 2), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector2f type");
		Vector2f* ptr = (Vector2f*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<Vector3f>(const Vector3f& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::Float, 3), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector3f type");
		Vector3f* ptr = (Vector3f*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<Vector4f>(const Vector4f& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::Float, 4), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector4f type");
		Vector4f* ptr = (Vector4f*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<Color>(const Color& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::Float, 4), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Color type");
		Color* ptr = (Color*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<byte>(const byte& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::UByte, 1), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector4<byte> type");
		byte* ptr = (byte*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<Vector2<byte>>(const Vector2<byte>& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::UByte, 2), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector2<byte> type");
		Vector2<byte>* ptr = (Vector2<byte>*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<Vector3<byte>>(const Vector3<byte>& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::UByte, 3), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector3<byte> type");
		Vector3<byte>* ptr = (Vector3<byte>*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<Vector4<byte>>(const Vector4<byte>& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::UByte, 4), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector4<byte> type");
		Vector4<byte>* ptr = (Vector4<byte>*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<Vector2i>(const Vector2i& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::Int, 2), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector2i type");
		Vector2i* ptr = (Vector2i*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<Vector3i>(const Vector3i& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::Int, 3), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector3i type");
		Vector3i* ptr = (Vector3i*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<Vector4i>(const Vector4i& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::Int, 4), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with Vector4i type");
		Vector4i* ptr = (Vector4i*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<int>(const int& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::Int, 1), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with int type");
		int* ptr = (int*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<unsigned int>(const unsigned int& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::UInt, 1), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with uint32_t type");
		unsigned int* ptr = (unsigned int*)m_Ptr;
		*ptr = obj;
	}

	template<>
	inline void AttributeSetter::Set<float>(const float& obj)
	{
		BLT_ASSERT(ValidateAttribute(m_AttribIndex, DataType::Float, 1), "Attribute with index " + std::to_string(m_AttribIndex) + " was incompatible with float type");
		float* ptr = (float*)m_Ptr;
		*ptr = obj;
	}

}