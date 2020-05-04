#include "bltpch.h"
#include "UniformLink.h"

namespace Bolt
{

	UniformLinkContainer::UniformLinkContainer(id_t shaderId, int location)
		: m_ShaderId(shaderId), m_Location(location)
	{
	
	}

	UniformLinkContainer::~UniformLinkContainer()
	{
	
	}

	id_t UniformLinkContainer::ShaderId() const
	{
		return m_ShaderId;
	}

	int UniformLinkContainer::UniformLocation() const
	{
		return m_Location;
	}

}