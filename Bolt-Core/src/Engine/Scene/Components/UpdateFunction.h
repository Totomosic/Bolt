#pragma once
#include "Component.h"

namespace Bolt
{

	class EntityHandle;

	class BLT_API UpdateFunction : public Component<UpdateFunction>
	{
	private:
		std::function<void(const EntityHandle&)> m_Function;

	public:
		UpdateFunction(const std::function<void(const EntityHandle&)>& fn);

		void Execute(const EntityHandle& entity) const;
	};

}
