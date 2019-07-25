#pragma once
#include "VertexFactory.h"

namespace Bolt
{

	class BLT_API GridFactory : public VertexFactory
	{
	public:
		float Width;
		float Depth;
		int xVertices;
		int zVertices;
		Bolt::Color Color;

	public:
		GridFactory(float width, float depth, int xVertices = 2, int zVertices = 2, const Bolt::Color& color = Color::White);

		ModelData GenerateVertices() const override;

	};

}