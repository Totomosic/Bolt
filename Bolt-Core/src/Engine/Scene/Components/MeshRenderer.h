#pragma once
#include "Component.h"
#include "..\GameObject.h"

namespace Bolt
{

	class BLT_API MeshRenderer : public Component
	{
	public:
		Mesh Mesh;

	public:
		MeshRenderer(Bolt::Mesh mesh);

		Cuboid GetMeshBounds() const;
		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}