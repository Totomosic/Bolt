#pragma once
#include "Bolt-Graphics.h"
#include "Renderable.h"
#include "RenderMethod.h"
#include "RenderPass.h"
#include "..\Scene\__Scene__.h"
#include "RenderMethods\DefaultRenderMethod.h"

namespace Bolt
{

	class BLT_API Renderer
	{
	protected:
		std::unique_ptr<RenderMethod> m_Method;

	public:
		Renderer(std::unique_ptr<RenderMethod>&& renderMethod = std::make_unique<DefaultRenderMethod>());

		virtual void Begin(const RenderPass* renderPass) const;
		virtual void Render(const RenderPass* renderPass) const;
		virtual void End(const RenderPass* renderPass) const;

	private:
		void RenderGameObjects(const RenderPass* renderPass, const std::vector<GameObject*>& objects, const Matrix4f& viewMatrix, const Matrix4f& projMatrix, const UniformManager& uniforms) const;
		std::vector<GameObject*> PerformClipping(const std::vector<GameObject*>& objects, const std::vector<Plane>& clippingPlanes) const;
		bool TestClipPlane(const Plane& plane, const Cuboid& box) const;

	};

}