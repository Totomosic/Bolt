#include "RubicsCube.h"

namespace RubicsCube
{

	RubicsCube::RubicsCube()
	{
	
	}

	RubicsCube::RubicsCube(ObjectFactory& factory)
		: m_Cubes(), m_Root(factory.Instantiate()), m_Rotator(factory.Instantiate())
	{
		m_Rotator->MakeChildOf(m_Root);
		for (int z = -1; z <= 1; z++)
		{
			for (int y = -1; y <= 1; y++)
			{
				for (int x = -1; x <= 1; x++)
				{
					GameObject* cube = factory.Cuboid(0.95f, 0.95f, 0.95f, Color::Random(), Transform({ (float)x, (float)y, (float)z }));
					cube->MakeChildOf(m_Root);
					m_Cubes.push_back(cube);
				}
			}
		}
	}

	GameObject* RubicsCube::Root() const
	{
		return m_Root;
	}

}