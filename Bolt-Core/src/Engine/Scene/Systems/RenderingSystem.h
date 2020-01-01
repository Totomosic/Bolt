#pragma once
#include "System.h"

namespace Bolt
{

	class BLT_API RenderingSystem : public System<RenderingSystem>
	{
	private:
		EntityHandle m_ActiveCamera;

	public:
		RenderingSystem();

		const EntityHandle& GetActiveCamera() const;
		void SetActiveCamera(const EntityHandle& camera);

		virtual void Update(EntityManager& manager, TimeDelta delta) override;
	};

}