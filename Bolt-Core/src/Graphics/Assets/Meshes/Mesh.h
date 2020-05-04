#pragma once
#include "Materials/Material.h"
#include "Model.h"
#include "../AssetHandle.h"

namespace Bolt
{

	struct BLT_API Mesh
	{
	public:
		struct BLT_API ModelGroup
		{
		public:
			AssetHandle<const Bolt::Model> Model = nullptr;
			Matrix4f Transform = Matrix4f::Identity();
			std::vector<int> MaterialIndices = { 0 }; // size should equal the number of index buffers that the model has
		};

	public:
		std::vector<ModelGroup> Models;
		std::vector<std::unique_ptr<Material>> Materials;

	public:
		Mesh();
		Mesh(const Mesh& other);
		Mesh& operator=(const Mesh& other);
		Mesh(Mesh&& other);
		Mesh& operator=(Mesh&& other);
		~Mesh() = default;

		Cuboid ComputeBoundingBox(const Matrix4f& transform = Matrix4f::Identity()) const;

	};

}