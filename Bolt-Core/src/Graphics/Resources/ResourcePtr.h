#pragma once
#include "Types.h"

namespace Bolt
{

	template<typename T>
	class BLT_API ResourcePtr
	{
	public:
		using pointer = T*;

	private:
		pointer m_Ptr = nullptr;
		bool m_OwnsPtr = false;

	public:
		ResourcePtr()
			: m_Ptr(nullptr), m_OwnsPtr(false)
		{
		
		}

		ResourcePtr(pointer ptr, bool ownsPtr = false)
			: m_Ptr(ptr), m_OwnsPtr(ownsPtr)
		{
		
		}

		ResourcePtr(const ResourcePtr<T>& other)
		{
			if (other.m_OwnsPtr)
			{
				m_Ptr = (pointer)other.m_Ptr->Clone();
				m_OwnsPtr = true;
			}
			else
			{
				m_Ptr = other.m_Ptr;
				m_OwnsPtr = false;
			}
		}

		ResourcePtr<T>& operator=(const ResourcePtr<T>& other)
		{
			if (other.m_OwnsPtr)
			{
				m_Ptr = (pointer)other.m_Ptr->Clone();
				m_OwnsPtr = true;
			}
			else
			{
				m_Ptr = other.m_Ptr;
				m_OwnsPtr = false;
			}
			return *this;
		}

		ResourcePtr(ResourcePtr<T>&& other)
		{
			pointer ptr = m_Ptr;
			bool ownsPtr = m_OwnsPtr;
			m_Ptr = other.m_Ptr;
			m_OwnsPtr = other.m_OwnsPtr;
			other.m_Ptr = ptr;
			other.m_OwnsPtr = ownsPtr;
		}

		ResourcePtr<T>& operator=(ResourcePtr<T>&& other)
		{
			pointer ptr = m_Ptr;
			bool ownsPtr = m_OwnsPtr;
			m_Ptr = other.m_Ptr;
			m_OwnsPtr = other.m_OwnsPtr;
			other.m_Ptr = ptr;
			other.m_OwnsPtr = ownsPtr;
			return *this;
		}

		~ResourcePtr()
		{
			if (m_OwnsPtr && m_Ptr != nullptr)
			{
				// Delete Ptr
				delete m_Ptr;
			}
		}

		template<typename Other>
		operator ResourcePtr<Other>()
		{
			ResourcePtr<Other> result = ResourcePtr<Other>((Other*)m_Ptr, m_OwnsPtr);
			if (m_OwnsPtr)
			{
				// TEST? either return a copy of m_Ptr or return a ResourcePtr that doesnt own m_Ptr
				m_Ptr = nullptr;
			}
			return std::move(result);
		}

		template<typename Other>
		operator ResourcePtr<Other>() const
		{
			ResourcePtr<Other> result = ResourcePtr<Other>((Other*)m_Ptr, m_OwnsPtr);
			if (m_OwnsPtr)
			{
				result.Set((Other*)m_Ptr->Clone());
				BLT_WARN("Copied resource when converting ResourcePtr");
			}
			return std::move(result);
		}

		ResourcePtr<T> Clone() const
		{
			pointer newResource = (pointer)m_Ptr->Clone();
			return ResourcePtr<T>(newResource, true);
		}

		pointer Get() const { return m_Ptr; }
		bool OwnsPtr() const { return m_OwnsPtr; }
		void Set(pointer ptr) { m_Ptr = ptr; }

		pointer operator*() const { return m_Ptr; }
		pointer operator->() const { return m_Ptr; }

		bool operator==(const ResourcePtr<T>& other) const { return m_Ptr == other.m_Ptr; }
		bool operator!=(const ResourcePtr<T>& other) const { return !(*this == other); }

	};

}