#include "bltpch.h"
#include "EntityManager.h"

namespace Bolt
{

	// ==================================================================================================================
	// ENTITY HANDLE
	// ==================================================================================================================

	EntityHandle::EntityHandle(EntityManager* manager, const Entity& entity)
		: m_Manager(manager), m_Entity(entity)
	{
	
	}

	EntityHandle::EntityHandle() : EntityHandle(nullptr, {})
	{

	}

	EntityHandle::operator Entity() const
	{
		return m_Entity;
	}

	EntityHandle::operator bool() const
	{
		return IsValid();
	}

	bool EntityHandle::IsValid() const
	{
		return m_Manager != nullptr && m_Manager->IsValid(m_Entity);
	}

	void EntityHandle::Destroy()
	{
		BLT_ASSERT(IsValid(), "Entity is not valid");
		m_Manager->Destroy(m_Entity);
		m_Manager = nullptr;
	}

	ComponentHandle<Transform> EntityHandle::GetTransform() const
	{
		return GetComponent<Transform>();
	}

	bool EntityHandle::operator==(const EntityHandle& other) const
	{
		return m_Entity.Id == other.m_Entity.Id && m_Manager == other.m_Manager;
	}

	bool EntityHandle::operator!=(const EntityHandle& other) const
	{
		return !(*this == other);
	}

	// ==================================================================================================================
	// ENTITY MANAGER
	// ==================================================================================================================

	EntityManager::EntityManager()
		: m_IndexCounter(0), m_FreeList(), m_ComponentMasks(), m_ComponentPools(),
		m_Bus(std::make_unique<EventBus>()), m_OnEntityCreated(m_Bus->GetEmitter<EntityCreated>()), m_OnEntityDestroyed(m_Bus->GetEmitter<EntityDestroyed>())
	{
		m_OnEntityDestroyed.AddEventListener([this](Event<EntityDestroyed>& e)
			{
				if (e.Data.Entity.IsValid())
				{
					DestroyInternal(e.Data.Entity);
				}
			}, ListenerPriority::Low);
	}

	EntityManager::~EntityManager()
	{
		for (EntityHandle entity : GetEntities())
		{
			entity.Destroy();
		}
	}

	EventEmitter<EntityCreated>& EntityManager::OnEntityCreated()
	{
		return m_OnEntityCreated;
	}

	EventEmitter<EntityDestroyed>& EntityManager::OnEntityDestroyed()
	{
		return m_OnEntityDestroyed;
	}

	const EventBus& EntityManager::Bus() const
	{
		return *m_Bus;
	}

	EventBus& EntityManager::Bus()
	{
		return *m_Bus;
	}

	size_t EntityManager::EntityCount() const
	{
		return m_ComponentMasks.size() - m_FreeList.size();
	}

	size_t EntityManager::EntityCapacity() const
	{
		return m_ComponentMasks.size();
	}

	bool EntityManager::IsValid(const Entity& entity) const
	{
		return ValidEntity(entity);
	}

	EntityHandle EntityManager::Get(const Entity& entity) const
	{
		return EntityHandle((EntityManager*)this, entity);
	}

	EntityHandle EntityManager::Create()
	{
		EntityId id = GetNextEntityId();
		AccomodateEntity(id);
		Entity e = { id };
		return EntityHandle(this, e);
	}

	EntityHandle EntityManager::CreateFrom(const EntityHandle& original)
	{
		BLT_ASSERT(ValidEntity(original), "Invalid entity");
		EntityHandle clone = Create();
		auto& mask = m_ComponentMasks[GetEntityIndex(original)];
		for (size_t i = 0; i < m_ComponentHelpers.size(); i++)
		{
			BaseComponentHelper* helper = m_ComponentHelpers[i].get();
			if (helper != nullptr && mask.test(i))
			{
				helper->CopyTo(original, clone);
			}
		}
		return clone;
	}

	void EntityManager::Destroy(const Entity& entity)
	{
		OnEntityDestroyed().Emit({ Get(entity) });
	}

	void EntityManager::Clear()
	{
		m_IndexCounter = 0;
		m_FreeList.clear();
		m_ComponentMasks.clear();
		m_ComponentPools.clear();
		m_ComponentHelpers.clear();
	}

	void EntityManager::DestroyInternal(const Entity& entity)
	{
		BLT_ASSERT(ValidEntity(entity), "Invalid entity");
		size_t index = GetEntityIndex(entity);
		auto& mask = m_ComponentMasks[index];
		for (size_t i = 0; i < m_ComponentHelpers.size(); i++)
		{
			BaseComponentHelper* helper = m_ComponentHelpers[i].get();
			if (helper != nullptr && mask.test(i))
			{
				helper->RemoveComponent(Get(entity));
			}
		}
		m_ComponentMasks[index].reset();
		m_FreeList.push_back((EntityId)index);
	}

	void EntityManager::AccomodateEntity(EntityManager::EntityId id)
	{
		if (m_ComponentMasks.size() <= id)
		{
			m_ComponentMasks.resize((size_t)id + 1);
			for (const std::unique_ptr<BasePool>& pool : m_ComponentPools)
			{
				if (pool != nullptr)
				{
					pool->Expand((size_t)id + 1);
				}
			}
		}
	}

	size_t EntityManager::GetEntityIndex(const Entity& entity) const
	{
		return entity.Id;
	}

	bool EntityManager::ValidFamily(const BaseComponent::Family family) const
	{
		return family < m_ComponentPools.size();
	}

	bool EntityManager::ValidEntity(const Entity& entity) const
	{
		return entity.Id != Entity::InvalidId && GetEntityIndex(entity) < m_ComponentMasks.size() && std::find(m_FreeList.begin(), m_FreeList.end(), entity.Id) == m_FreeList.end();
	}

	EntityManager::EntityId EntityManager::GetNextEntityId()
	{
		if (m_FreeList.empty())
		{
			return m_IndexCounter++;
		}
		EntityId id = m_FreeList.back();
		m_FreeList.pop_back();
		return id;
	}

	// ==================================================================================================================
	// VIEWS
	// ==================================================================================================================

	EntityManager::ViewIterator::ViewIterator(const EntityManager* manager, EntityManager::ComponentMask componentMask, EntityManager::EntityId index)
		: m_Manager(manager), m_Mask(componentMask), m_Index(index)
	{
		Next();
	}

	EntityManager::ViewIterator::ViewIterator(const EntityManager* manager, EntityManager::EntityId index) : ViewIterator(manager, {}, index)
	{

	}

	EntityManager::ViewIterator& EntityManager::ViewIterator::operator++()
	{
		m_Index++;
		Next();
		return *this;
	}

	EntityHandle EntityManager::ViewIterator::operator*() const
	{
		return m_Manager->Get({ m_Index });
	}

	EntityHandle EntityManager::ViewIterator::operator*()
	{
		return m_Manager->Get({ m_Index });
	}

	bool EntityManager::ViewIterator::operator==(const ViewIterator& other) const
	{
		return m_Index == other.m_Index;
	}

	bool EntityManager::ViewIterator::operator!=(const ViewIterator& other) const
	{
		return !(*this == other);
	}

	void EntityManager::ViewIterator::Next()
	{
		while (m_Index < m_Manager->EntityCapacity() && !IsValid())
		{
			m_Index++;
		}
	}

	bool EntityManager::ViewIterator::IsValid() const
	{
		return m_Manager->IsValid({ m_Index }) && (m_Manager->m_ComponentMasks[m_Index] & m_Mask) == m_Mask;
	}

	EntityManager::BaseView::BaseView(const EntityManager* manager, EntityManager::ComponentMask componentMask)
		: m_Manager(manager), m_Mask(componentMask)
	{
	}

	EntityManager::BaseView::Iterator EntityManager::BaseView::begin()
	{
		return Iterator(m_Manager, m_Mask, 0);
	}

	EntityManager::BaseView::Iterator EntityManager::BaseView::end()
	{
		return Iterator(m_Manager, m_Mask, m_Manager->EntityCapacity());
	}

	const EntityManager::BaseView::Iterator EntityManager::BaseView::begin() const
	{
		return Iterator(m_Manager, m_Mask, 0);
	}

	const EntityManager::BaseView::Iterator EntityManager::BaseView::end() const
	{
		return Iterator(m_Manager, m_Mask, m_Manager->EntityCapacity());
	}

}