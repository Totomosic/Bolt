#pragma once
#include "VertexFactory.h"

namespace Bolt
{

	class BLT_API RectangleFactory : public VertexFactory
	{
	public:
		float Width;
		float Height;
		Bolt::Color Color;

	public:
		RectangleFactory(float width = 1.0f, float height = 1.0f, const Bolt::Color& color = Color::White);

		MeshData GenerateVertices() const override;

	};

}