#include "bltpch.h"
#include "RenderingSystem.h"

#include "../Components/Transform.h"
#include "../Components/Camera.h"
#include "Graphics/Resources/Meshes/Mesh.h"

#include "Engine/Renderer/GlobalRenderer.h"
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
			for (const EntityHandle& entity : manager.GetEntitiesWith<Transform, Mesh>())
			{
				ComponentHandle<Transform> transform = entity.GetComponent<Transform>();
				ComponentHandle<Mesh> mesh = entity.GetComponent<Mesh>();

				for (const auto& group : mesh->Models)
				{
					for (int i = 0; i < group.MaterialIndices.size(); i++)
					{
						RenderData data;
						data.Transform = transform->TransformMatrix() * group.Transform;
						data.Vertices = group.Model->Data().Vertices.get();
						data.Indices = group.Model->Data().Indices->GetIndexBuffer(i).get();

						RenderGroup renderGroup;
						renderGroup.Material = mesh->Materials[group.MaterialIndices[i]].get();
						renderGroup.Renderables.push_back(data);

						RenderPass pass;
						pass.RenderTarget = Graphics::Get().DefaultFramebuffer();
						pass.RenderGroups.push_back(renderGroup);

						GlobalRenderer::Render(pass, context, c);
					}					
				}				
			}
		}
	}

}