#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "Graphics/Resources/Meshes/Mesh.h"

namespace Bolt
{

	class BLT_API MeshRenderer : public Component
	{
	public:
		Mesh Mesh;

	public:
		MeshRenderer(Bolt::Mesh mesh);

		std::vector<Cuboid> GetMeshBounds() const;
		Cuboid GetOverallMeshBounds() const;

		std::unique_ptr<Component> Clone() const override;

	};

}