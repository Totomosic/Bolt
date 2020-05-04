#pragma once
#include "Graphics/Assets/Meshes/Model.h"
#include "Graphics/Assets/AssetHandle.h"

namespace Bolt
{

	class AssetManager;

	class BLT_API BasicModels
	{
	private:
		AssetHandle<Model> m_SquareModel;
		AssetHandle<Model> m_CircleModel;
		AssetHandle<Model> m_CubeModel;
		AssetHandle<Model> m_SphereModel;

	public:
		BasicModels(AssetManager* manager);

		const AssetHandle<Model>& Square() const;
		const AssetHandle<Model>& Circle() const;
		const AssetHandle<Model>& Cube() const;
		const AssetHandle<Model>& Sphere() const;

	};

}