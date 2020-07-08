#pragma once
#include "EntityManager.h"

namespace Bolt
{

	class BLT_API ScopedEntityHandle
	{
	private:
		EntityHandle m_Entity;

	public:
		ScopedEntityHandle() = default;
		ScopedEntityHandle(const EntityHandle& entity);
		ScopedEntityHandle(const ScopedEntityHandle& other) = delete;
		ScopedEntityHandle& operator=(const ScopedEntityHandle& other) = delete;
		ScopedEntityHandle(ScopedEntityHandle&& other);
		ScopedEntityHandle& operator=(ScopedEntityHandle&& other);
		~ScopedEntityHandle();

		operator EntityHandle() const;

		const EntityHandle& Get() const;
	};

}