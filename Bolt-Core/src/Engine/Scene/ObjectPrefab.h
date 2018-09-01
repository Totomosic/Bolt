#pragma once
#include "Bolt-Graphics.h"
#include "Transform.h"
#include "Components\ComponentManager.h"

namespace Bolt
{

	// Base Class for GameObject, that contains components but is not associated with a layer
	class BLT_API ObjectPrefab
	{
	protected:
		Transform m_Transform;
		ComponentManager m_Components;

	public:
		ObjectPrefab(Transform transform = Transform());
		ObjectPrefab(const ObjectPrefab& other) = delete;
		ObjectPrefab& operator=(const ObjectPrefab& other) = delete;
		ObjectPrefab(ObjectPrefab&& other);
		ObjectPrefab& operator=(ObjectPrefab&& other);
		~ObjectPrefab() = default;

		const Transform& transform() const;
		Transform& transform();
		const ComponentManager& Components() const;
		ComponentManager& Components();

	};

}