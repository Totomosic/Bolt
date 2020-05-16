#include "bltpch.h"
#include "UpdateFunction.h"
#include "../EntityManager.h"

namespace Bolt
{

	UpdateFunction::UpdateFunction(const std::function<void(const EntityHandle&)>& fn)
		: m_Function(fn)
	{
	}

	void UpdateFunction::Execute(const EntityHandle& entity) const
	{
		m_Function(entity);
	}

}
