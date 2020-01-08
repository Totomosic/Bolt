#pragma once
#include "VertexFactory.h"

namespace Bolt
{

	class BLT_API CuboidFactory : public VertexFactory
	{
	public:
		float Width;
		float Height;
		float Depth;
		Bolt::Color Color;

	public:
		CuboidFactory(float width = 1.0f, float height = 1.0f, float depth = 1.0f, const Bolt::Color& color = Color::White);

		ModelData GenerateVertices() const override;

	};

}