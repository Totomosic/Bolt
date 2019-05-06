#pragma once
#include "Core/Math/Maths.h"

namespace Bolt
{

	struct BLT_API RenderCamera
	{
	public:
		Matrix4f ViewMatrix;
		Matrix4f ProjectionMatrix;
	};

	struct BLT_API RenderCameraRef
	{
	public:
		const Matrix4f* ViewMatrix = nullptr;
		const Matrix4f* ProjectionMatrix = nullptr;
	};

}