#pragma once
#include "Core/Time/Time.h"
#include "Core/Events/EventManager.h"
#include "RenderContext.h"

namespace Bolt
{

	class BLT_API ContextManager
	{
	public:
		ContextManager(AppContext* context);
	};

	class BLT_API AppContext
	{
	private:
		ContextManager m_Context;
		Time m_Time;
		EventManager m_Events;
		std::unique_ptr<RenderContext> m_RenderContext;

	public:
		AppContext(bool createRenderContext, const WindowCreateInfo& createInfo);

		bool HasRenderContext() const;
		const RenderContext& GetRenderContext() const;
		RenderContext& GetRenderContext();

		EventManager& GetEventManager();
		Time& GetTime();

	};

}