#include "Types.h"

#include "GameObject.h"
#include "Layer.h"

namespace Bolt
{

	GameObject::GameObject() : ObjectPrefab(),
		m_Id(GameObject::InvalidID), m_Parent(nullptr), m_Layer(nullptr), m_Children(), m_Tags()
	{
	
	}

	GameObject::GameObject(GameObject&& other)
		: m_Id(other.m_Id), m_Layer(other.m_Layer)
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
		std::vector<blt::string> tags = m_Tags;
		m_Tags = other.m_Tags;
		m_Parent = other.m_Parent;
		other.m_Tags = tags;
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
		std::vector<blt::string> tags = m_Tags;
		m_Tags = other.m_Tags;
		m_Parent = other.m_Parent;
		m_Id = other.m_Id;
		m_Transform = std::move(other.m_Transform);
		m_Layer = other.m_Layer;
		m_Components = std::move(other.m_Components);
		other.m_Tags = tags;
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

	GameObject* GameObject::Parent() const
	{
		return m_Parent;
	}

	Layer* GameObject::GetLayer() const
	{
		return m_Layer;
	}

	id_t GameObject::Id() const
	{
		return m_Id;
	}

	const std::vector<blt::string>& GameObject::Tags() const
	{
		return m_Tags;
	}

	void GameObject::MakeChildOf(GameObject* parent)
	{
		if (parent == nullptr)
		{
			MakeStandalone();
			return;
		}
		m_Parent = parent;
		m_Transform.SetParent(&parent->transform());
	}

	void GameObject::MakeStandalone()
	{
		m_Parent = nullptr;
		m_Transform.SetParent(nullptr);
	}

	void GameObject::AddTag(const blt::string& tag)
	{
		BLT_ASSERT(m_Layer != nullptr, "Must be a part of a valid layer");
		m_Layer->GameObjects().TagGameObject(tag, this);
	}

	void GameObject::RemoveTag(const blt::string& tag)
	{
		BLT_ASSERT(m_Layer != nullptr, "Must be a part of a valid layer");
		m_Layer->GameObjects().RemoveTag(this, tag);
	}

	void GameObject::RemoveAllTags()
	{
		BLT_ASSERT(m_Layer != nullptr, "Must be a part of a valid layer");
		m_Layer->GameObjects().RemoveAllTags(this);
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

	void GameObject::Transfer(XMLserializer& backend, bool isWriting)
	{
		ObjectPrefab::Transfer(backend, isWriting);
		BLT_TRANSFER(backend, m_Id);
		BLT_TRANSFER(backend, m_Parent);
		BLT_TRANSFER(backend, m_Children);
		BLT_TRANSFER(backend, m_Layer);
	}

	void GameObject::SetLayer(Layer* layer)
	{
		m_Layer = layer;
	}

	void GameObject::AddTagPrivate(const blt::string& tag)
	{
		m_Tags.push_back(tag);
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

	GameObject* GameObject::Instantiate(Layer* layer, const ObjectPrefab* prefab)
	{
		GameObject* object = layer->AddGameObject(GameObject());
		for (const auto& pair : prefab->Components().m_ComponentMap)
		{
			object->Components().AddComponent(pair.first, pair.second->Clone());
		}
		return object;
	}

	GameObject* GameObject::Instantiate(Layer* layer, const ObjectPrefab* prefab, Transform transform)
	{
		GameObject* object = Instantiate(layer, prefab);
		object->transform() = std::move(transform);
		return object;
	}

	GameObject* GameObject::Instantiate(Layer* layer, const ObjectPrefab* prefab, GameObject* parent)
	{
		GameObject* object = Instantiate(layer, prefab);
		object->MakeChildOf(parent);
		return object;
	}

	GameObject* GameObject::Instantiate(Layer* layer, const ObjectPrefab* prefab, GameObject* parent, Transform transform)
	{
		GameObject* object = Instantiate(layer, prefab, std::move(transform));
		object->MakeChildOf(parent);
		return object;
	}

}