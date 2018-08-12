#pragma once
#include "Component.h"
#include "..\GameObject.h"

namespace Bolt
{

	class BLT_API MeshRenderer : public Component
	{
	public:
		Mesh Mesh;
		std::function<void(Bolt::Mesh&)> Deleter;

	public:
		MeshRenderer(Bolt::Mesh mesh, const std::function<void(Bolt::Mesh&)>& deleter = [](Bolt::Mesh& mesh) {});
		~MeshRenderer() override;

		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}