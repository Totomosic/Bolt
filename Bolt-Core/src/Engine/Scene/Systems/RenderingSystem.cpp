#include "bltpch.h"
#include "RenderingSystem.h"

#include "../Components/Transform.h"
#include "../Components/Camera.h"
#include "Graphics/Assets/Meshes/Mesh.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Graphics.h"

namespace Bolt
{
	RenderingSystem::RenderingSystem()
		: m_ActiveCamera(), m_Framebuffer(nullptr)
	{
	}

	const EntityHandle& RenderingSystem::GetActiveCamera() const
	{
		return m_ActiveCamera;
	}

	void RenderingSystem::SetActiveCamera(const EntityHandle& camera)
	{
		m_ActiveCamera = camera;
	}

	const Framebuffer* RenderingSystem::GetRenderTarget() const
	{
		if (m_Framebuffer == nullptr)
		{
			return Graphics::Get().DefaultFramebuffer();
		}
		return m_Framebuffer;
	}

	void RenderingSystem::SetRenderTarget(const Framebuffer* renderTarget)
	{
		m_Framebuffer = renderTarget;
	}

	void RenderingSystem::Update(EntityManager& manager, TimeDelta delta)
	{
		if (m_ActiveCamera.IsValid())
		{
			ComponentHandle<Transform> cameraTransform = m_ActiveCamera.GetComponent<Transform>();
			ComponentHandle<Camera> cameraComponent = m_ActiveCamera.GetComponent<Camera>();
			RenderCamera c;
			c.ProjectionMatrix = cameraComponent->GetProjectionMatrix();
			c.ViewMatrix = cameraTransform->InverseTransformMatrix();

			RenderingContext context;
			for (EntityHandle entity : manager.GetEntitiesWith<Transform, LightSource>())
			{
				ComponentHandle<Transform> transform = entity.GetComponent<Transform>();
				ComponentHandle<LightSource> lightSource = entity.GetComponent<LightSource>();
				LightSourceInstance instance;
				instance.LightData = *lightSource;
				instance.Position = transform->Position();
				instance.Direction = transform->Forward();
				context.LightSources.push_back(instance);
			}
			if (context.LightSources.empty())
			{
				LightSourceInstance sun;
				sun.Position = { 0, 1000, 0 };
				context.LightSources.push_back(sun);
			}

			Renderer& renderer = Graphics::Get().GetRenderer();
			renderer.BeginScene(GetRenderTarget(), c, context);

			for (const EntityHandle& entity : manager.GetEntitiesWith<Transform, Mesh>())
			{
				ComponentHandle<Transform> transform = entity.GetComponent<Transform>();
				ComponentHandle<Mesh> mesh = entity.GetComponent<Mesh>();
				renderer.Submit(*mesh, transform->TransformMatrix());
			}

			renderer.EndScene();
		}
	}

}