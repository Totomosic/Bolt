#pragma once
#include "VertexFactory.h"

namespace Bolt
{

	class BLT_API EllipseFactory : public VertexFactory
	{
	public:
		float Width;
		float Height;
		Color Color;
		int VerticesPerRev;

	public:
		EllipseFactory(float width = 1.0f, float height = 1.0f, int verticesPerRev = 360, const Bolt::Color& color = Color::White);

		ModelData GenerateVertices() const override;

	};

}