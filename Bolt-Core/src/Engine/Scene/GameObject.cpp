#include "GameObject.h"
#include "Layer.h"

namespace Bolt
{

	GameObject::GameObject()
		: m_Id(GameObject::InvalidID), m_Transform(), m_Components(this), m_Parent(nullptr), m_Layer(nullptr)
	{
	
	}

	GameObject::GameObject(GameObject&& other)
		: m_Id(other.m_Id), m_Transform(std::move(other.m_Transform)), m_Components(std::move(other.m_Components)), m_Layer(other.m_Layer)
	{
		for (GameObject* child : other.m_Children)
		{
			child->m_Parent = this;
		}
		if (other.m_Parent != nullptr)
		{
			for (int i = 0; i < other.m_Parent->m_Children.size(); i++)
			{
				if (other.m_Parent->m_Children[i] == &other)
				{
					other.m_Parent->m_Children[i] = this;
				}
			}
		}
		m_Parent = other.m_Parent;
		m_Components.SetGameObject(this);
	}

	GameObject& GameObject::operator=(GameObject&& other)
	{
		for (GameObject* child : other.m_Children)
		{
			child->m_Parent = this;
		}
		if (other.m_Parent != nullptr)
		{
			for (int i = 0; i < other.m_Parent->m_Children.size(); i++)
			{
				if (other.m_Parent->m_Children[i] == &other)
				{
					other.m_Parent->m_Children[i] = this;
				}
			}
		}
		m_Parent = other.m_Parent;
		m_Id = other.m_Id;
		m_Transform = std::move(other.m_Transform);
		m_Layer = other.m_Layer;
		m_Components = std::move(other.m_Components);
		m_Components.SetGameObject(this);
		return *this;
	}

	GameObject::~GameObject()
	{
		if (m_Parent != nullptr)
		{
			for (int i = 0; i < m_Parent->m_Children.size(); i++)
			{
				if (m_Parent->m_Children[i] == this)
				{
					m_Parent->m_Children.erase(m_Parent->m_Children.begin() + i);
					break;
				}
			}
		}
		for (GameObject* child : m_Children)
		{
			child->m_Parent = nullptr;
		}
	}

	const Transform& GameObject::transform() const
	{
		return m_Transform;
	}

	Transform& GameObject::transform()
	{
		return m_Transform;
	}

	const ComponentManager& GameObject::Components() const
	{
		return m_Components;
	}

	ComponentManager& GameObject::Components()
	{
		return m_Components;
	}

	GameObject* GameObject::Parent() const
	{
		return m_Parent;
	}

	Layer* GameObject::GetLayer() const
	{
		return m_Layer;
	}

	id_t GameObject::ID() const
	{
		return m_Id;
	}

	void GameObject::MakeChildOf(GameObject* parent)
	{
		m_Parent = parent;
		m_Transform.SetParent(&parent->transform());
	}

	void GameObject::MakeStandalone()
	{
		m_Parent = nullptr;
		m_Transform.SetParent(nullptr);
	}

	void GameObject::SetID(id_t id)
	{
		m_Id = id;
	}

	void GameObject::Update()
	{
		for (int i = 0; i < ComponentManager::MAX_COMPONENTS; i++)
		{
			Component* c = m_Components.m_ComponentArray[i];
			if (c != nullptr)
			{
				c->Update();
			}
		}
	}

	void GameObject::LateUpdate()
	{
		for (int i = 0; i < ComponentManager::MAX_COMPONENTS; i++)
		{
			Component* c = m_Components.m_ComponentArray[i];
			if (c != nullptr)
			{
				c->LateUpdate();
			}
		}
	}

	void GameObject::SetLayer(Layer* layer)
	{
		m_Layer = layer;
	}

	GameObject* GameObject::Instantiate(Layer* layer, Transform transform)
	{
		GameObject* object = layer->AddGameObject(GameObject());
		object->transform() = std::move(transform);
		return object;
	}

	GameObject* GameObject::Instantiate(Layer* layer, float x, float y, float z, const Quaternion& orientation, const Vector3f& scale)
	{
		return Instantiate(layer, Transform(Vector3f(x, y, z), orientation, scale));
	}

	GameObject* GameObject::Instantiate(Layer* layer, const GameObject* prefab)
	{
		GameObject* object = layer->AddGameObject(GameObject());
		for (const auto& pair : prefab->Components().m_ComponentMap)
		{
			object->Components().AddComponent(pair.first, pair.second->Clone());
		}
		return object;
	}

	GameObject* GameObject::Instantiate(Layer* layer, const GameObject* prefab, Transform transform)
	{
		GameObject* object = Instantiate(layer, prefab);
		object->transform() = std::move(transform);
		return object;
	}

	GameObject* GameObject::Instantiate(Layer* layer, const GameObject* prefab, GameObject* parent)
	{
		GameObject* object = Instantiate(layer, prefab);
		object->MakeChildOf(parent);
		return object;
	}

	GameObject* GameObject::Instantiate(Layer* layer, const GameObject* prefab, GameObject* parent, Transform transform)
	{
		GameObject* object = Instantiate(layer, prefab, std::move(transform));
		object->MakeChildOf(parent);
		return object;
	}

}