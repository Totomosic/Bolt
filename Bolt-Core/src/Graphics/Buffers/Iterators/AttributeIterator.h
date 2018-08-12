#pragma once
#include "..\BufferLayout.h"

namespace Bolt
{

	class VertexArray;

	// Iterates over all vertices at a specific attribute
	template<typename T>
	class BLT_API AttributeIterator
	{
	public:
		typedef AttributeIterator<T> Iterator;

	private:
		byte* m_Ptr;

		const BufferLayout* m_Layout;
		const VertexArray* m_Array;
		int m_AttribIndex;

	public:
		AttributeIterator() = default;
		AttributeIterator(byte* ptr, const VertexArray* varray, const BufferLayout* bufferLayout, int attribIndex)
			: m_Ptr(ptr), m_Layout(bufferLayout), m_Array(varray), m_AttribIndex(attribIndex)
		{
		
		}

		AttributeIterator(const Iterator& other) = delete;
		Iterator& operator=(const Iterator& other) = delete;

		AttributeIterator(Iterator&& other)
			: m_Ptr(other.m_Ptr), m_Layout(other.m_Layout), m_Array(other.m_Array), m_AttribIndex(other.m_AttribIndex)
		{
			other.m_Ptr = nullptr;
		}

		Iterator& operator=(Iterator&& other)
		{
			m_Ptr = other.m_Ptr;
			m_Layout = other.m_Layout;
			m_Array = other.m_Array;
			m_AttribIndex = other.m_AttribIndex;
			other.m_Ptr = nullptr;
			return *this;
		}

		~AttributeIterator()
		{
			if (m_Ptr != nullptr)
			{
				m_Array->FreeAttributeIterator(*this);
			}
		}

		inline const T* Get() const { return (T*)m_Ptr; }
		inline T* Get() { return (T*)m_Ptr; }
		inline const T* operator->() const { return (T*)m_Ptr; }
		inline T* operator->() { return (T*)m_Ptr; }
		inline const T& operator*() const { return *(T*)m_Ptr; }
		inline T& operator*() { return *(T*)m_Ptr; }

		inline int AttributeIndex() const { return m_AttribIndex; }

		Iterator& operator++()
		{
			m_Ptr += m_Layout->Stride() * 1;
			return *this;
		}

		Iterator& operator++(int)
		{
			m_Ptr += m_Layout->Stride() * 1;
			return *this;
		}

		Iterator& operator--()
		{
			m_Ptr -= m_Layout->Stride() * 1;
			return *this;
		}

		Iterator& operator--(int)
		{
			m_Ptr -= m_Layout->Stride() * 1;
			return *this;
		}

	};

}