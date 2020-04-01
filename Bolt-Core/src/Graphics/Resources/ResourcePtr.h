#pragma once
#include "bltpch.h"

namespace Bolt
{

	template<typename T>
	class BLT_API ResourcePtr
	{
	private:
		T* m_Ptr;
		int* m_RefCount;

	public:
		ResourcePtr() : ResourcePtr(nullptr, false)
		{
		
		}

		ResourcePtr(T* ptr, bool ownsPtr = false)
			: m_Ptr(ptr), m_RefCount(nullptr)
		{
			if (ownsPtr)
			{
				m_RefCount = new int(1);
			}
		}

		ResourcePtr(const ResourcePtr<T>& other)
			: m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount)
		{
			IncrementRefCount();
		}

		ResourcePtr<T>& operator=(const ResourcePtr<T>& other)
		{
			DecrementRefCount();
			m_Ptr = other.m_Ptr;
			m_RefCount = other.m_RefCount;
			IncrementRefCount();
			return *this;
		}

		ResourcePtr(ResourcePtr<T>&& other)
			: m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount)
		{
			IncrementRefCount();
		}

		ResourcePtr<T>& operator=(ResourcePtr<T>&& other)
		{
			T* ptr = m_Ptr;
			int* ref = m_RefCount;
			m_Ptr = other.m_Ptr;
			m_RefCount = other.m_RefCount;
			other.m_Ptr = ptr;
			other.m_RefCount = ref;
			TestRefCount();
			return *this;
		}

		~ResourcePtr()
		{
			DecrementRefCount();
		}

		template<typename Other>
		operator ResourcePtr<Other>() const
		{
			ResourcePtr<Other> result = ResourcePtr<Other>((Other*)m_Ptr, false);
			IncrementRefCount();
			result.m_RefCount = m_RefCount;
			return result;
		}

		ResourcePtr<T> Clone() const
		{
			T* newResource = (T*)m_Ptr->Clone().release();
			BLT_CORE_WARN("Cloned Resource ({}) through ResourcePtr", typeid(T).name());
			return ResourcePtr<T>(newResource, true);
		}

		T* Get() const { return m_Ptr; }
		bool OwnsPtr() const { return m_RefCount != nullptr; }

		T& operator*() const { return *m_Ptr; }
		T* operator->() const { return m_Ptr; }

		bool operator==(const ResourcePtr<T>& other) const { return m_Ptr == other.m_Ptr; }
		bool operator==(std::nullptr_t ptr) const { return m_Ptr == ptr; }
		bool operator!=(const ResourcePtr<T>& other) const { return !(*this == other); }
		bool operator!=(std::nullptr_t ptr) const { return !(*this == ptr); }

		template<typename>
		friend class ResourcePtr;

	private:
		void IncrementRefCount() const
		{
			if (m_RefCount != nullptr)
			{
				(*m_RefCount)++;
			}
		}

		void DecrementRefCount()
		{
			if (m_RefCount != nullptr)
			{
				(*m_RefCount)--;
				TestRefCount();
			}
		}

		void TestRefCount()
		{
			if (m_RefCount != nullptr && (*m_RefCount) <= 0 && m_Ptr != nullptr)
			{
				BLT_DELETE m_Ptr;
			}
		}
	};

}

namespace std
{

	template<typename T>
	struct hash<Bolt::ResourcePtr<T>>
	{
	public:
		size_t operator()(const Bolt::ResourcePtr<T>& ptr) const
		{
			return std::hash<const void*>()((const void*)ptr.Get());
		}
	};

}