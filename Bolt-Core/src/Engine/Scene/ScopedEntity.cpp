#include "bltpch.h"
#include "ScopedEntity.h"

namespace Bolt
{

	ScopedEntityHandle::ScopedEntityHandle(const EntityHandle& entity)
		: m_Entity(entity)
	{
	}

	ScopedEntityHandle::ScopedEntityHandle(ScopedEntityHandle&& other)
		: m_Entity(other.m_Entity)
	{
		other.m_Entity = EntityHandle();
	}

	ScopedEntityHandle& ScopedEntityHandle::operator=(ScopedEntityHandle&& other)
	{
		EntityHandle temp = m_Entity;
		m_Entity = other.m_Entity;
		other.m_Entity = temp;
		return *this;
	}

	ScopedEntityHandle::~ScopedEntityHandle()
	{
		if (m_Entity)
		{
			m_Entity.Destroy();
		}
	}

	ScopedEntityHandle::operator EntityHandle() const
	{
		return m_Entity;
	}

	const EntityHandle& ScopedEntityHandle::Get() const
	{
		return m_Entity;
	}

}
