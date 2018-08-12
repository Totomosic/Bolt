#pragma once
#include "Bolt-Graphics.h"
#include "Renderable.h"
#include "RenderBatch.h"
#include "GLState.h"

namespace Bolt
{

	struct BLT_API RenderMethod
	{
	public:
		virtual void operator()(const RenderBatch& batch, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix) const = 0;
	};

}