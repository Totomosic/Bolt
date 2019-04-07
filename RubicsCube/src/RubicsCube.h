#pragma once
#include "Bolt.h"

namespace RubicsCube
{

	class RubicsCube
	{
	private:
		std::vector<GameObject*> m_Cubes;
		GameObject* m_Root;
		GameObject* m_Rotator;

	public:
		RubicsCube();
		RubicsCube(ObjectFactory& factory);

		GameObject* Root() const;

	};

}