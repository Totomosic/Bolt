#include "bltpch.h"
#include "ModelData.h"

namespace Bolt
{

	ModelMapping ModelData::Map() const
	{
		return { Vertices->Map(), Indices->Map() };
	}

}