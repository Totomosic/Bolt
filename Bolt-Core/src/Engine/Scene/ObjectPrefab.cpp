#include "ObjectPrefab.h"

namespace Bolt
{

	ObjectPrefab::ObjectPrefab(Transform transform)
		: m_Transform(std::move(transform)), m_Components(this)
	{
		
	}

	ObjectPrefab::ObjectPrefab(ObjectPrefab&& other)
		: m_Components(std::move(other.m_Components)), m_Transform(std::move(other.m_Transform))
	{
		m_Components.SetGameObject(this);
	}

	ObjectPrefab& ObjectPrefab::operator=(ObjectPrefab&& other)
	{
		m_Transform = std::move(other.m_Transform);
		m_Components = std::move(other.m_Components);
		m_Components.SetGameObject(this);
		return *this;
	}

	const Transform& ObjectPrefab::transform() const
	{
		return m_Transform;
	}

	Transform& ObjectPrefab::transform()
	{
		return m_Transform;
	}

	const ComponentManager& ObjectPrefab::Components() const
	{
		return m_Components;
	}

	ComponentManager& ObjectPrefab::Components()
	{
		return m_Components;
	}

}