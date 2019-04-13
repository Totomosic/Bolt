#pragma once
#include "RenderContext.h"

namespace Bolt
{

	class BLT_API AppContext
	{
	private:
		RenderContext m_Context;

	public:
		AppContext(const WindowCreateInfo& createInfo);

		const RenderContext& GetRenderContext() const;
		RenderContext& GetRenderContext();

	};

}