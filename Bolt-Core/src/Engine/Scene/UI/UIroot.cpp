#include "bltpch.h"

#include "UIroot.h"
#include "../Layer.h"

namespace Bolt
{

	UIroot::UIroot() : UIroot(nullptr, nullptr)
	{

	}

	UIroot::UIroot(Layer* layer, GameObject* object) : UIelement(),
		m_Factory(*layer)
	{
		m_Root = this;
		m_Object = object;
	}

	UIroot::~UIroot()
	{
		
	}

	const ObjectFactory& UIroot::GetFactory() const
	{
		return m_Factory;
	}

	ObjectFactory& UIroot::GetFactory()
	{
		return m_Factory;
	}

}