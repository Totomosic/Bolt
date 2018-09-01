#include "SceneGraph.h"

namespace Bolt
{

	SceneGraph::SceneGraph()
		: m_ObjectCollection(nullptr)
	{
	
	}

	void SceneGraph::SetObjectCollection(const ObjectCollection* objects)
	{
		m_ObjectCollection = objects;
	}

}