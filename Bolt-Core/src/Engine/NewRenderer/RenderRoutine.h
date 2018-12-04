#pragma once
#include "RenderGroup.h"

namespace Bolt
{

	class BLT_API RenderRoutine
	{
	public:
		virtual void operator()(const RenderGroup& group, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix);
	};

}