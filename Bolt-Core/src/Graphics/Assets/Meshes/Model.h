#pragma once
#include "Materials/Material.h"
#include "Mesh.h"
#include "../AssetHandle.h"

namespace Bolt
{

	struct BLT_API Model
	{
	public:
		struct BLT_API MeshGroup
		{
		public:
			AssetHandle<Bolt::Mesh> Mesh = nullptr;
			Matrix4f Transform = Matrix4f::Identity();
			std::vector<int> MaterialIndices = { 0 }; // size should equal the number of index buffers that the mesh has
		};

	public:
		std::vector<MeshGroup> Meshes;
		std::vector<std::unique_ptr<Material>> Materials;

	public:
		Model();
		Model(const Model& other);
		Model& operator=(const Model& other);
		Model(Model&& other);
		Model& operator=(Model&& other);
		~Model() = default;

		Cuboid ComputeBoundingBox(const Matrix4f& transform = Matrix4f::Identity()) const;

	};

}