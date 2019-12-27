#include "bltpch.h"
#include "RenderingSystem.h"

#include "../Components/Transform.h"
#include "../Components/Camera.h"
#include "Graphics/Resources/Meshes/Mesh.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Graphics.h"

namespace Bolt
{

	void RenderingSystem::Update(EntityManager& manager, TimeDelta delta)
	{
		EntityHandle camera;
		for (EntityHandle c : manager.GetEntitiesWith<Transform, Camera>())
		{
			camera = c;
			break;
		}
		if (camera.IsValid())
		{
			ComponentHandle<Transform> cameraTransform = camera.GetComponent<Transform>();
			ComponentHandle<Camera> cameraComponent = camera.GetComponent<Camera>();
			RenderCamera c;
			c.ProjectionMatrix = cameraComponent->GetProjectionMatrix();
			c.ViewMatrix = cameraTransform->InverseTransformMatrix();
			RenderingContext context;
			LightSource sun;
			sun.Position = { 0, 1000, 0 };
			context.LightSources.push_back(sun);

			Renderer& renderer = Graphics::Get().GetRenderer();
			renderer.BeginScene(Graphics::Get().DefaultFramebuffer(), c, context);

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