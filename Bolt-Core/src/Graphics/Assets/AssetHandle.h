#pragma once
#include "bltpch.h"

namespace Bolt
{

	template<typename T>
	class BLT_API AssetHandle
	{
	private:
		T* m_Ptr;
		int* m_RefCount;

	public:
		AssetHandle() : AssetHandle(nullptr, false)
		{
		
		}

		AssetHandle(T* ptr, bool ownsPtr = false)
			: m_Ptr(ptr), m_RefCount(nullptr)
		{
			if (ownsPtr)
			{
				m_RefCount = new int(1);
			}
		}

		AssetHandle(const AssetHandle<T>& other)
			: m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount)
		{
			IncrementRefCount();
		}

		AssetHandle<T>& operator=(const AssetHandle<T>& other)
		{
			DecrementRefCount();
			m_Ptr = other.m_Ptr;
			m_RefCount = other.m_RefCount;
			IncrementRefCount();
			return *this;
		}

		AssetHandle(AssetHandle<T>&& other)
			: m_Ptr(other.m_Ptr), m_RefCount(other.m_RefCount)
		{
			IncrementRefCount();
		}

		AssetHandle<T>& operator=(AssetHandle<T>&& other)
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

		~AssetHandle()
		{
			DecrementRefCount();
		}

		template<typename Other>
		operator AssetHandle<Other>() const
		{
			AssetHandle<Other> result = AssetHandle<Other>((Other*)m_Ptr, false);
			IncrementRefCount();
			result.m_RefCount = m_RefCount;
			return result;
		}

		AssetHandle<T> Clone() const
		{
			T* newResource = (T*)m_Ptr->Clone().release();
			BLT_CORE_WARN("Cloned Resource ({}) through AssetHandle", typeid(T).name());
			return AssetHandle<T>(newResource, true);
		}

		T* Get() const { return m_Ptr; }
		bool OwnsPtr() const { return m_RefCount != nullptr; }

		T& operator*() const { return *m_Ptr; }
		T* operator->() const { return m_Ptr; }

		bool operator==(const AssetHandle<T>& other) const { return m_Ptr == other.m_Ptr; }
		bool operator==(std::nullptr_t ptr) const { return m_Ptr == ptr; }
		bool operator!=(const AssetHandle<T>& other) const { return !(*this == other); }
		bool operator!=(std::nullptr_t ptr) const { return !(*this == ptr); }

		template<typename>
		friend class AssetHandle;

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
	struct hash<Bolt::AssetHandle<T>>
	{
	public:
		size_t operator()(const Bolt::AssetHandle<T>& ptr) const
		{
			return std::hash<const void*>()((const void*)ptr.Get());
		}
	};

}