#pragma once
#include "VertexFactory.h"

namespace Bolt
{

	class BLT_API LineFactory : public VertexFactory
	{
	public:
		Vector3f Direction;
		float Length;
		Color Color;

	public:
		LineFactory(const Vector3f& direction, float length, const Bolt::Color& color = Color::White);

		ModelData GenerateVertices() const override;

	};

}