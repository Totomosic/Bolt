#include "bltpch.h"
#include "System.h"

namespace Bolt
{

	BaseSystem::Family BaseSystem::s_FamilyCounter = 0;


	BaseSystem::BaseSystem()
		: m_IsEnabled(true)
	{
	}

	bool BaseSystem::IsEnabled() const
	{
		return m_IsEnabled;
	}

	void BaseSystem::SetEnabled(bool enabled)
	{
		if (enabled != m_IsEnabled)
		{
			SetEnabledInternal(enabled);
		}
	}

	void BaseSystem::SetEnabledInternal(bool enabled)
	{
		m_IsEnabled = enabled;
	}

}