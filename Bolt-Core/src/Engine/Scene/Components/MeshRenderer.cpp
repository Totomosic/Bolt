#include "MeshRenderer.h"

namespace Bolt
{

	MeshRenderer::MeshRenderer(Bolt::Mesh mesh)
		: Mesh(std::move(mesh))
	{
	
	}

	void MeshRenderer::Update()
	{
		for (int i = 0; i < Mesh::MAX_MATERIALS; i++)
		{
			for (auto& pair : Mesh.Materials[i].Textures.Animators)
			{
				if (!pair.second->IsPaused())
				{
					pair.second->Update(Time::DeltaTime());
				}
			}
		}
	}

	std::unique_ptr<Component> MeshRenderer::Clone() const
	{
		return std::make_unique<MeshRenderer>(Mesh);
	}

}