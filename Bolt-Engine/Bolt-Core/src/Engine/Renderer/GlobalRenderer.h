#pragma once
#include "RenderPass.h"
#include "RenderingContext.h"
#include "RenderCamera.h"
#include "RenderRoutine.h"

namespace Bolt
{

	class BLT_API GlobalRenderer
	{
	private:
		static std::unique_ptr<RenderRoutine> s_RenderRoutine;

	public:
		static void Render(const RenderPass& renderPass, const RenderingContext& context, const RenderCamera& camera);

	};

}