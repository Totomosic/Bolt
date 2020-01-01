#include "bltpch.h"
#include "Layer.h"

#include "Systems/RenderingSystem.h"
#include "Components/Camera.h"
#include "Components/Transform.h"

namespace Bolt
{

	Layer::Layer()
		: m_Entities(), m_Systems(m_Entities), m_IsActive(true), m_RenderingSystem(), m_UISystem()
	{
		m_RenderingSystem = m_Systems.Register<RenderingSystem>();
	}

	EntityFactory Layer::GetFactory()
	{
		return EntityFactory(*this);
	}

	const EntityManager& Layer::Entities() const
	{
		return m_Entities;
	}

	EntityManager& Layer::Entities()
	{
		return m_Entities;
	}

	const SystemRegistry& Layer::Systems() const
	{
		return m_Systems;
	}

	SystemRegistry& Layer::Systems()
	{
		return m_Systems;
	}

	UIManager& Layer::GetUI()
	{
		if (m_UISystem == nullptr)
		{
			m_UISystem = Systems().Register<UIManager>(&m_Entities);
			if (!m_IsActive)
			{
				m_UISystem->Disable();
			}
		}
		return *m_UISystem;
	}

	const EntityHandle& Layer::GetActiveCamera() const
	{
		return m_RenderingSystem->GetActiveCamera();
	}

	void Layer::SetActiveCamera(const EntityHandle& entity)
	{
		BLT_ASSERT(ValidateCamera(entity), "Entity is not a valid camera. Entity requires a Transform and Camera component");
		m_RenderingSystem->SetActiveCamera(entity);
	}

	bool Layer::IsEnabled() const
	{
		return m_IsActive;
	}

	void Layer::Enable()
	{
		if (!m_IsActive)
		{
			m_IsActive = true;
			if (m_UISystem)
			{
				m_UISystem->Enable();
			}
		}
	}

	void Layer::Disable()
	{
		if (m_IsActive)
		{
			m_IsActive = false;
			if (m_UISystem)
			{
				m_UISystem->Disable();
			}
		}
	}

	void Layer::Update(TimeDelta delta)
	{
		if (m_IsActive)
		{
			m_Systems.UpdateAll(delta);
			if (m_UISystem != nullptr)
			{
				Systems().Update(m_UISystem, delta);
			}
		}
	}

	void Layer::Render(TimeDelta delta)
	{
		if (m_IsActive)
		{
			Systems().Update(m_RenderingSystem, delta);
		}
	}

	bool Layer::ValidateCamera(const EntityHandle& entity) const
	{
		return entity.IsValid() && entity.HasComponents<Transform, Camera>();
	}

}