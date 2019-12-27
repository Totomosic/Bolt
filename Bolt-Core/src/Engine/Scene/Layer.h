#pragma once
#include "SystemRegistry.h"
#include "Systems/RenderingSystem.h"
#include "EntityFactory.h"

namespace Bolt
{

	class BLT_API Layer
	{
	private:
		EntityManager m_Entities;
		SystemRegistry m_Systems;

		SystemRegistry::SystemPtr<RenderingSystem> m_RenderingSystem;

	public:
		Layer();

		EntityFactory GetFactory();

		const EntityManager& Entities() const;
		EntityManager& Entities();
		const SystemRegistry& Systems() const;
		SystemRegistry& Systems();

		void Update(TimeDelta delta);
		void Render(TimeDelta delta);

	private:
		bool ValidateCamera(const EntityHandle& entity) const;
	};

}