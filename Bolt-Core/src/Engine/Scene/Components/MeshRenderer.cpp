#include "bltpch.h"
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
		std::vector<Cuboid> result;
		for (int i = 0; i < Mesh.Models.size(); i++)
		{
			const Mesh::ModelGroup& model = Mesh.Models[i];
			const ModelData& info = model.Model->Data();
			Cuboid bounds = { { info.Bounds.MinX, info.Bounds.MinY, info.Bounds.MinZ }, { info.Bounds.MaxX, info.Bounds.MaxY, info.Bounds.MaxZ } };
			bounds.Min = (model.Transform * Vector4f(bounds.Min, 1.0f)).xyz();
			bounds.Max = (model.Transform * Vector4f(bounds.Max, 1.0f)).xyz();
			bounds.Min.x = std::min(bounds.Min.x, bounds.Max.x);
			bounds.Min.y = std::min(bounds.Min.y, bounds.Max.y);
			bounds.Min.z = std::min(bounds.Min.z, bounds.Max.z);
			bounds.Max.x = std::max(bounds.Min.x, bounds.Max.x);
			bounds.Max.y = std::max(bounds.Min.y, bounds.Max.y);
			bounds.Max.z = std::max(bounds.Min.z, bounds.Max.z);
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
		for (Cuboid& bounds : GetMeshBounds())
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
		return std::make_unique<MeshRenderer>(Mesh);
	}

}