#pragma once
#include "VertexFactory.h"

namespace Bolt
{

	class BLT_API SphereFactory : public VertexFactory
	{
	public:
		float Radius;
		int SectorCount;
		Bolt::Color Color;

	public:
		SphereFactory(float radius, int sectorCount = 360, const Bolt::Color& color = Color::White);

		MeshData GenerateVertices() const override;

	};

}