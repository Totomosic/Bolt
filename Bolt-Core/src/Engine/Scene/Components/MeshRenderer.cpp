#include "Types.h"

#include "MeshRenderer.h"

namespace Bolt
{

	MeshRenderer::MeshRenderer(Bolt::Mesh mesh)
		: Mesh(std::move(mesh))
	{
	
	}

	std::vector<Cuboid> MeshRenderer::GetMeshBounds() const
	{
		BLT_ASSERT(Mesh.Models.size() > 0, "No models in mesh");
		Vector3f position = gameObject()->transform().Position();
		std::vector<Cuboid> result;
		for (int i = 0; i < Mesh.Models.size(); i++)
		{
			const Mesh::ModelGroup& model = Mesh.Models[i];
			const ModelData& info = model.Model->Data();
			float width = info.Bounds.MaxX - info.Bounds.MinX;
			float height = info.Bounds.MaxY - info.Bounds.MinY;
			float depth = info.Bounds.MaxZ - info.Bounds.MinZ;
			Vector3f meshScale = Vector3f(model.Transform.Element(0, 0), model.Transform.Element(1, 1), model.Transform.Element(2, 2));
			Vector3f size = Vector3f(width, height, depth) * gameObject()->transform().Scale() * meshScale;
			Cuboid bounds = { position - size / 2, position + size / 2 };
			result.push_back(bounds);
		}
		return result;
	}

	Cuboid MeshRenderer::GetOverallMeshBounds() const
	{
		float MAX_VALUE = +1e20;
		float MIN_VALUE = -1e20;
		BLT_ASSERT(Mesh.Models.size() > 0, "No models in mesh");
		Cuboid result;
		result.Min = Vector3f(MAX_VALUE, MAX_VALUE, MAX_VALUE);
		result.Max = Vector3f(MIN_VALUE, MIN_VALUE, MIN_VALUE);
		for (Cuboid bounds : GetMeshBounds())
		{
			result.Min.x = std::min(result.Min.x, bounds.Min.x);
			result.Min.y = std::min(result.Min.y, bounds.Min.y);
			result.Min.z = std::min(result.Min.z, bounds.Min.z);
			result.Max.x = std::max(result.Max.x, bounds.Max.x);
			result.Max.y = std::max(result.Max.y, bounds.Max.y);
			result.Max.z = std::max(result.Max.z, bounds.Max.z);
		}
		return result;
	}

	std::unique_ptr<Component> MeshRenderer::Clone() const
	{
		return std::make_unique<MeshRenderer>(Mesh.Clone());
	}

}