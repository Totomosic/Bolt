#include "Types.h"

#include "MeshRenderer.h"

namespace Bolt
{

	MeshRenderer::MeshRenderer(Bolt::Mesh mesh)
		: Mesh(mesh)
	{
	
	}

	Cuboid MeshRenderer::GetMeshBounds() const
	{
		Vector3f position = gameObject()->transform().Position();
		const Mesh::ModelGroup& model = Mesh.Models[0];
		const ModelData& info = model.Model->Data();
		float width = info.Bounds.MaxX - info.Bounds.MinX;
		float height = info.Bounds.MaxY - info.Bounds.MinY;
		float depth = info.Bounds.MaxZ - info.Bounds.MinZ;
		Vector3f meshScale = Vector3f(model.Transform.Element(0, 0), model.Transform.Element(1, 1), model.Transform.Element(2, 2));
		Vector3f size = Vector3f(width, height, depth) * gameObject()->transform().Scale() * meshScale;
		Cuboid result = { position - size / 2, position + size / 2 };
		return result;
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