#pragma once
#include "Types.h"

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

		T GetNextId() const
		{
			if (m_CurrentMaxId < m_MaxId)
			{
				return m_CurrentMaxId++;
			}
			if (m_AvailableIds.size() > 0)
			{
				return m_AvailableIds.front();
			}
			BLT_ASSERT(false, "No available Id");
			return m_MaxId;
		}

		void ReleaseId(id_t id) const
		{
			m_AvailableIds.push_back(id);
		}

		void Reset() const
		{
			m_CurrentMaxId = m_MinId;
			m_AvailableIds.clear();
		}

	};

}