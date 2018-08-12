#pragma once
#include "../RenderMethod.h"

namespace Bolt
{

	struct BLT_API DefaultRenderMethod : public RenderMethod
	{
	public:
		void operator()(const RenderBatch& batch, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix) const override;

	};

}