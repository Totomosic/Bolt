#pragma once
#include "Nodes/MaterialValue.h"

namespace Bolt
{

	class BLT_API MaterialGraphContext
	{
	public:
		struct BLT_API GraphContextValue
		{
		public:
			MaterialValue Value;
			bool IsUsed = false;
		};

	public:
		MaterialGraphContext();

		MaterialValue VertexPosition();
		MaterialValue VertexNormal();
		MaterialValue VertexTexCoord();
		MaterialValue VertexColor();
		MaterialValue VertexTangent();

		MaterialValue ModelMatrix();
		MaterialValue ViewMatrix();
		MaterialValue ProjectionMatrix();

		MaterialValue WorldSpacePosition();
		MaterialValue ViewSpacePosition();
		MaterialValue ScreenSpacePosition();

		MaterialValue CameraPosition();
		MaterialValue CameraDirection();


	};

}