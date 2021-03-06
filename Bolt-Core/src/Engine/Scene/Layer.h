#pragma once
#include "SystemRegistry.h"
#include "Systems/RenderingSystem.h"
#include "Systems/UpdateSystem.h"
#include "Systems/UIManager.h"
#include "EntityFactory.h"

namespace Bolt
{

	class BLT_API Layer
	{
	private:
		EntityManager m_Entities;
		SystemRegistry m_Systems;
		bool m_IsActive;

		SystemRegistry::SystemPtr<RenderingSystem> m_RenderingSystem;
		SystemRegistry::SystemPtr<UpdateSystem> m_UpdateSystem;
		SystemRegistry::SystemPtr<UIManager> m_UISystem;

	public:
		Layer();

		EntityFactory GetFactory();

		const EntityManager& Entities() const;
		EntityManager& Entities();
		const SystemRegistry& Systems() const;
		SystemRegistry& Systems();

		UIManager& GetUI();
		RenderingSystem& GetRenderer();

		const EntityHandle& GetActiveCamera() const;
		void SetActiveCamera(const EntityHandle& entity);

		bool IsEnabled() const;
		void Enable();
		void Disable();

		void Update(TimeDelta delta);
		void Render(TimeDelta delta);

	private:
		bool ValidateCamera(const EntityHandle& entity) const;
	};

}