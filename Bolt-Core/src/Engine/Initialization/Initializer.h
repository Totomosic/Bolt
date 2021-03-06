#pragma once
#include "bltpch.h"
#include "Engine/EngineCreateInfo.h"

namespace Bolt
{

	class BLT_API Initializer
	{
	public:
		Initializer() = delete;

		static void PreOpenGL(const EngineCreateInfo& createInfo);

	};

}