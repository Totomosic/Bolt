#pragma once
#include "System.h"
#include "Graphics/Framebuffer.h"

namespace Bolt
{

	class BLT_API RenderingSystem : public System<RenderingSystem>
	{
	private:
		EntityHandle m_ActiveCamera;
		const Framebuffer* m_Framebuffer;

	public:
		RenderingSystem();

		const EntityHandle& GetActiveCamera() const;
		void SetActiveCamera(const EntityHandle& camera);
		const Framebuffer* GetRenderTarget() const;
		void SetRenderTarget(const Framebuffer* renderTarget);

		virtual void Update(EntityManager& manager, TimeDelta delta) override;
	};

}