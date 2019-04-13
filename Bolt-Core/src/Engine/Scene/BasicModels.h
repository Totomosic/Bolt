#pragma once
#include "Graphics/Resources/Meshes/__Meshes__.h"

namespace Bolt
{

	class BLT_API BasicModels
	{
	private:
		ResourcePtr<Model> m_SquareModel;
		ResourcePtr<Model> m_CircleModel;
		ResourcePtr<Model> m_CubeModel;

	public:
		static BasicModels& Get();

	public:
		BasicModels();

		const ResourcePtr<Model>& Square() const;
		const ResourcePtr<Model>& Circle() const;
		const ResourcePtr<Model>& Cube() const;

	};

}