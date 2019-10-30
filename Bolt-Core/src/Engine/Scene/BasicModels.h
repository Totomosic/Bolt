#pragma once
#include "Graphics/Resources/Meshes/Model.h"
#include "Graphics/Resources/ResourcePtr.h"

namespace Bolt
{

	class ResourceManager;

	class BLT_API BasicModels
	{
	private:
		ResourcePtr<Model> m_SquareModel;
		ResourcePtr<Model> m_CircleModel;
		ResourcePtr<Model> m_CubeModel;
		ResourcePtr<Model> m_SphereModel;

	public:
		static BasicModels& Get();

	public:
		BasicModels(ResourceManager* manager);

		const ResourcePtr<Model>& Square() const;
		const ResourcePtr<Model>& Circle() const;
		const ResourcePtr<Model>& Cube() const;
		const ResourcePtr<Model>& Sphere() const;

	};

}