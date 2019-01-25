#pragma once
#include "Types.h"
#include "..\Functions.h"

namespace Bolt
{

	template<typename T>
	class BLT_API IdManager
	{
	private:
		T m_MinId;
		T m_MaxId;
		mutable T m_CurrentMaxId;
		mutable std::vector<T> m_AvailableIds;

	public:
		IdManager(T min, T max)
			: m_MinId(min), m_MaxId(max), m_CurrentMaxId(min), m_AvailableIds()
		{
			
		}

		bool IsIdAvailable(T id) const
		{
			if (id > m_CurrentMaxId)
			{
				return true;
			}
			auto it = std::find(m_AvailableIds.begin(), m_AvailableIds.end(), id);
			if (it != m_AvailableIds.end())
			{
				return true;
			}
			return false;
		}

		T PeekNextId() const
		{
			if (m_CurrentMaxId < m_MaxId)
			{
				return m_CurrentMaxId;
			}
			if (m_AvailableIds.size() > 0)
			{
				T id = m_AvailableIds.back();
				return id;
			}
			BLT_ASSERT(false, "No available Id");
			return m_MaxId;
		}

		T GetNextId() const
		{
			if (m_CurrentMaxId < m_MaxId)
			{
				return m_CurrentMaxId++;
			}
			if (m_AvailableIds.size() > 0)
			{
				T id = m_AvailableIds.back();
				m_AvailableIds.pop_back();
				return id;
			}
			BLT_ASSERT(false, "No available Id");
			return m_MaxId;
		}

		void ReleaseId(id_t id) const
		{
			m_AvailableIds.push_back(id);
		}

		void Reset(T value = 0) const
		{
			m_CurrentMaxId = Clamp<T>(value, m_MinId, m_MaxId);
			m_AvailableIds.clear();
		}

		void SetNextAvailableId(T id)
		{
			m_CurrentMaxId = id;
		}

	};

}