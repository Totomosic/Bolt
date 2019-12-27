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
		EntityHandle camera = *manager.GetEntitiesWith<Transform, Camera>().begin();
		if (camera.IsValid())
		{
			ComponentHandle<Transform> cameraTransform = camera.GetComponent<Transform>();
			ComponentHandle<Camera> cameraComponent = camera.GetComponent<Camera>();
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