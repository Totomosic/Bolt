#pragma once
#include "RenderPass.h"
#include "RenderContext.h"
#include "RenderCamera.h"
#include "RenderRoutine.h"

namespace Bolt
{

	class BLT_API GlobalRenderer
	{
	private:
		static std::unique_ptr<RenderRoutine> s_RenderRoutine;

	public:
		static void Render(const RenderPass& renderPass, const RenderContext& context, const RenderCamera& camera);

	};

}