#include "bltpch.h"
#include "GameObject.h"
#include "Layer.h"
#include "Core/Time/Time.h"
#include "Core/Profiling/Profiling.h"

namespace Bolt
{

	GameObject::GameObject() : ObjectPrefab(),
		m_Id(GameObject::InvalidID), m_Parent(nullptr), m_Layer(nullptr), m_Children(), m_Tags(), m_TemporaryComponents()
	{
		m_Components.m_IsGameObject = true;
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
		std::vector<blt::string> tags = std::move(m_Tags);
		m_Tags = std::move(other.m_Tags);
		m_Parent = other.m_Parent;
		other.m_Tags = std::move(tags);
		m_Components.SetGameObject(this);
		m_Components.m_IsGameObject = true;
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
		std::vector<blt::string> tags = std::move(m_Tags);
		m_Tags = std::move(other.m_Tags);
		m_Parent = other.m_Parent;
		m_Id = other.m_Id;
		m_Transform = std::move(other.m_Transform);
		m_Layer = other.m_Layer;
		m_Components = std::move(other.m_Components);
		other.m_Tags = std::move(tags);
		m_Components.SetGameObject(this);
		m_Components.m_IsGameObject = true;
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
		BLT_PROFILE_FUNCTION();
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

	void GameObject::SetId(id_t id)
	{
		m_Id = id;
	}

	void GameObject::Update()
	{
		BLT_PROFILE_FUNCTION();
		std::vector<Component*> components = m_Components.GetComponents();
		for (Component* c : components)
		{
			BLT_PROFILE_SCOPE("void Component::Update(void)");
			c->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		BLT_PROFILE_FUNCTION();
		std::vector<Component*> components = m_Components.GetComponents();
		for (Component* c : components)
		{
			BLT_PROFILE_SCOPE("void Component::LateUpdate(void)");
			c->LateUpdate();
		}
		for (int i = m_TemporaryComponents.size() - 1; i >= 0; i--)
		{
			TempComponent& c = m_TemporaryComponents[i];
			c.TimeToDelete -= Time::Get().RenderingTimeline().DeltaTime();
			if (c.TimeToDelete <= 0)
			{
				m_Components.RemoveComponentById(c.component->Id());
				m_TemporaryComponents.erase(m_TemporaryComponents.begin() + i);
			}
		}
	}

	ObjectPrefab GameObject::GetPrefab() const
	{
		BLT_PROFILE_FUNCTION();
		ObjectPrefab prefab;
		prefab.transform().SetLocalPosition(transform().LocalPosition());
		prefab.transform().SetLocalOrientation(transform().LocalOrientation());
		prefab.transform().SetLocalScale(transform().LocalScale());
		for (ComponentManager::ComponentInfoPtr& c : Components().GetComponentsOrdered())
		{
			prefab.Components().AddComponent(c.type_hash, c.component->Clone());
		}
		return prefab;
	}

	void GameObject::Transfer(XMLserializer& backend, bool isWriting)
	{
		ObjectPrefab::Transfer(backend, isWriting);
		BLT_TRANSFER(backend, m_Id);
		BLT_TRANSFER(backend, m_Parent);
		BLT_TRANSFER(backend, m_Children);
		BLT_TRANSFER(backend, m_Layer);
	}

	void Destroy(Component* c, float timeToDelete)
	{
		BLT_PROFILE_FUNCTION();
		if (c->gameObject() == nullptr)
		{
			BLT_CORE_WARN("Deleting component that either is not attached to GameObject or has already been deleted, Id = {}", c->Id());
			return;
		}
		c->gameObject()->MarkComponentForDelete(c, timeToDelete);
	}

	void GameObject::SetLayer(Layer* layer)
	{
		m_Layer = layer;
	}

	void GameObject::MarkComponentForDelete(Component* component, float timeToDelete)
	{
		m_TemporaryComponents.push_back({ component, timeToDelete });
	}

	void GameObject::OnDestroy()
	{
		BLT_PROFILE_FUNCTION();
		MakeStandalone();
		m_TemporaryComponents.clear();
		m_Components.Clear();
		m_Transform.Reset();
		SetLayer(nullptr);
		SetId(GameObject::InvalidID);
	}

	void GameObject::AddTagPrivate(const blt::string& tag)
	{
		m_Tags.push_back(tag);
	}

	GameObject* GameObject::Instantiate(Layer* layer, Transform transform)
	{
		BLT_PROFILE_FUNCTION();
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
		return Instantiate(layer, prefab, Transform());
	}

	GameObject* GameObject::Instantiate(Layer* layer, const ObjectPrefab* prefab, Transform transform)
	{
		GameObject* object = layer->AddGameObject(GameObject());
		object->transform() = std::move(transform);
		std::vector<ComponentManager::ComponentInfoPtr>& components = prefab->Components().GetComponentsOrdered();
		for (ComponentManager::ComponentInfoPtr& component : components)
		{
			object->Components().AddComponent(component.type_hash, component.component->Clone());
		}
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