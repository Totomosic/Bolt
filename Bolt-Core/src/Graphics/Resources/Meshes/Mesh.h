#pragma once
#include "Materials\__Materials__.h"
#include "Model.h"

namespace Bolt
{

	struct BLT_API Mesh
	{
	public:
		struct BLT_API ModelGroup
		{
		public:
			const Model* Model = nullptr;
			Matrix4f Transform = Matrix4f::Identity();
			std::vector<int> MaterialIndices = { 0 }; // size should equal the number of index buffers that the model has
		};

		static constexpr int MAX_MATERIALS = 5;

	public:
		std::vector<ModelGroup> Models;
		Material Materials[MAX_MATERIALS];

	};

}