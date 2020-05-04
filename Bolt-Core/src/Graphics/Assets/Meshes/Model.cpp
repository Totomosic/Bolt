#include "bltpch.h"
#include "Model.h"

namespace Bolt
{

	Model::Model()
		: Meshes(), Materials()
	{
	
	}

	Model::Model(const Model& other)
		: Meshes(other.Meshes)
	{
		Materials.reserve(other.Materials.size());
		for (const auto& material : other.Materials)
		{
			Materials.push_back(material->Clone());
		}
	}

	Model& Model::operator=(const Model& other)
	{
		Meshes = other.Meshes;
		Materials.clear();
		Materials.resize(other.Materials.size());
		for (const auto& material : other.Materials)
		{
			Materials.push_back(material->Clone());
		}
		return *this;
	}

	Model::Model(Model&& other)
		: Meshes(std::move(other.Meshes)), Materials(std::move(other.Materials))
	{

	}

	Model& Model::operator=(Model&& other)
	{
		Meshes = std::move(other.Meshes);
		Materials = std::move(other.Materials);
		return *this;
	}

	Cuboid Model::ComputeBoundingBox(const Matrix4f& transform) const
	{
		Cuboid result;
		float minX = +INFINITY;
		float maxX = -INFINITY;
		float minY = +INFINITY;
		float maxY = -INFINITY;
		float minZ = +INFINITY;
		float maxZ = -INFINITY;
		for (const MeshGroup& group : Meshes)
		{
			Matrix4f completeTransform = transform * group.Transform;
			const Mesh& mesh = *group.Mesh;
			const MeshData& data = mesh.Data();
			const Cuboid& bounds = data.Bounds;
			Cuboid transformedBounds;
			transformedBounds.Min = (completeTransform * Vector4f(bounds.Min, 1.0f)).xyz();
			transformedBounds.Max = (completeTransform * Vector4f(bounds.Max, 1.0f)).xyz();
			minX = std::min({ transformedBounds.Min.x, transformedBounds.Max.x, minX });
			maxX = std::max({ transformedBounds.Min.x, transformedBounds.Max.x, maxX });
			minY = std::min({ transformedBounds.Min.y, transformedBounds.Max.y, minY });
			maxY = std::max({ transformedBounds.Min.y, transformedBounds.Max.y, maxY });
			minZ = std::min({ transformedBounds.Min.z, transformedBounds.Max.z, minZ });
			maxZ = std::max({ transformedBounds.Min.z, transformedBounds.Max.z, maxZ });
		}
		result.Min = { minX, minY, minZ };
		result.Max = { maxX, maxY, maxZ };
		return result;
	}

}