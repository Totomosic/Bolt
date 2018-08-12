#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	struct BLT_API Renderable
	{
	public:
		const Mesh* Mesh;
		Matrix4f Transform;

	};

}