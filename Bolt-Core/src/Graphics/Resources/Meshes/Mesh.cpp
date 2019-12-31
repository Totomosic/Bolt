#include "bltpch.h"
#include "Mesh.h"

namespace Bolt
{

	Mesh::Mesh()
		: Models(), Materials()
	{
	
	}

	Mesh::Mesh(const Mesh& other)
		: Models(other.Models), Materials()
	{
		Materials.reserve(other.Materials.size());
		for (const auto& material : other.Materials)
		{
			Materials.push_back(material->Clone());
		}
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		Models = other.Models;
		Materials.clear();
		Materials.resize(other.Materials.size());
		for (const auto& material : other.Materials)
		{
			Materials.push_back(material->Clone());
		}
		return *this;
	}

	Mesh::Mesh(Mesh&& other)
		: Models(std::move(other.Models)), Materials(std::move(other.Materials))
	{

	}

	Mesh& Mesh::operator=(Mesh&& other)
	{
		Models = std::move(other.Models);
		Materials = std::move(other.Materials);
		return *this;
	}

	Cuboid Mesh::ComputeBoundingBox(const Matrix4f& transform) const
	{
		Cuboid result;
		float minX = +INFINITY;
		float maxX = -INFINITY;
		float minY = +INFINITY;
		float maxY = -INFINITY;
		float minZ = +INFINITY;
		float maxZ = -INFINITY;
		for (const ModelGroup& group : Models)
		{
			Matrix4f completeTransform = transform * group.Transform;
			const Model& model = *group.Model;
			const ModelData& data = model.Data();
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