#include "Types.h"
#include "AppContext.h"

namespace Bolt
{

	AppContext::AppContext(const WindowCreateInfo& createInfo)
		: m_Context(this, createInfo)
	{
	
	}

	const RenderContext& AppContext::GetRenderContext() const
	{
		return m_Context;
	}

	RenderContext& AppContext::GetRenderContext()
	{
		return m_Context;
	}

}