#include "bltpch.h"
#include "Layer.h"

#include "Systems/RenderingSystem.h"
#include "Components/Camera.h"
#include "Components/Transform.h"

namespace Bolt
{

	Layer::Layer()
		: m_Entities(), m_Systems(m_Entities), m_ActiveCamera(), m_RenderingSystem()
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

	bool Layer::HasCamera() const
	{
		return m_ActiveCamera.IsValid();
	}

	EntityHandle Layer::GetCamera() const
	{
		return m_ActiveCamera;
	}

	void Layer::SetCamera(const EntityHandle& entity)
	{
		BLT_ASSERT(ValidateCamera(entity), "Entity is not a valid camera");
		m_ActiveCamera = entity;
	}

	void Layer::Update(TimeDelta delta)
	{
		m_Systems.UpdateAll(delta);
	}

	void Layer::Render(TimeDelta delta)
	{
		Systems().Update(m_RenderingSystem, delta);
	}

	bool Layer::ValidateCamera(const EntityHandle& entity) const
	{
		return entity.IsValid() && entity.HasComponents<Transform, Camera>();
	}

}