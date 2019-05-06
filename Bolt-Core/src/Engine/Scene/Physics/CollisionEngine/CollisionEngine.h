#pragma once
#include "../PhysObjCollection.h"

namespace Bolt
{

	class BLT_API CollisionEngine
	{
	private:
		PhysObjCollection* m_ObjectCollection;

	public:
		CollisionEngine(PhysObjCollection* objectCollection);

	};

}