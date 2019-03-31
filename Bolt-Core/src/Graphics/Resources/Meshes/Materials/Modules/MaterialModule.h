#pragma once
#include "../Shaders/Components/__Components__.h"
#include "../Shaders/ShaderFactory.h"

namespace Bolt
{

	class BLT_API MaterialModule
	{
	public:
		MaterialModule();

		virtual void Build(ShaderFactory& factory) = 0;

	};

}