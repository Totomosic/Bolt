#include "bltpch.h"
#include "UpdateSystem.h"
#include "../Components/UpdateFunction.h"

namespace Bolt
{

	void UpdateSystem::Update(EntityManager& entities, TimeDelta dt)
	{
		for (const EntityHandle& entity : entities.GetEntitiesWith<UpdateFunction>())
		{
			entity.GetComponent<UpdateFunction>()->Execute(entity);
		}
	}

}
