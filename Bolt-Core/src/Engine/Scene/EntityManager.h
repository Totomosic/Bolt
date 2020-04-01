#pragma once
#include "Components/Component.h"
#include "Core/Events/IdManager.h"
#include "Core/Events/EventBus.h"
#include "Core/Events/EventEmitter.h"
#include "Entity.h"
#include "Pool.h"

#include "Components/Transform.h"

/*
	Implementation of entity component system
	Heavily influenced by: https://github.com/alecthomas/entityx
*/

namespace Bolt
{

	class EntityManager;

	// ==================================================================================================================
	// COMPONENT HANDLE
	// ==================================================================================================================

	template<typename T, typename EM = EntityManager>
	class BLT_API ComponentHandle
	{
	private:
		EM* m_Manager;
		Entity m_Entity;

	private:
		ComponentHandle(EM* manager, const Entity& entity)
			: m_Manager(manager), m_Entity(entity)
		{

		}

		friend class EntityManager;

	public:
		ComponentHandle() : ComponentHandle(nullptr, {})
		{

		}

		inline operator bool() const { return IsValid(); }

		inline bool IsValid() const { return m_Manager != nullptr && m_Manager->IsValid(m_Entity); }
		inline T* Get() const
		{
			BLT_ASSERT(IsValid(), "Component handle is not valid");
			return m_Manager->template GetComponentPtr<T>(m_Entity);
		}

		T& operator*() const
		{
			return *Get();
		}

		T* operator->() const
		{
			return Get();
		}
	};

	// ==================================================================================================================
	// ENTITY HANDLE
	// ==================================================================================================================

	class BLT_API EntityHandle
	{
	private:
		EntityManager* m_Manager;
		Entity m_Entity;

	private:
		EntityHandle(EntityManager* manager, const Entity& entity);

		friend class EntityManager;

	public:
		EntityHandle();

		operator Entity() const;
		operator bool() const;
		bool IsValid() const;

		void Destroy();

		ComponentHandle<Transform> GetTransform() const;
		
		template<typename T>
		bool HasComponent() const;

		template<typename T1, typename ... T>
		bool HasComponents() const;

		template<typename T, typename ... Args>
		ComponentHandle<T> Assign(Args&& ... args) const;

		template<typename T>
		ComponentHandle<T> AssignFromCopy(const T& other) const;

		template<typename T>
		void Remove() const;

		template<typename T>
		ComponentHandle<T> GetComponent() const;

		template<typename ... T>
		std::tuple<ComponentHandle<T>...> GetComponents() const;

		bool operator==(const EntityHandle& other) const;
		bool operator!=(const EntityHandle& other) const;

	};

	// ==================================================================================================================
	// COMPONENT HELPERS
	// ==================================================================================================================

	class BLT_API BaseComponentHelper
	{
	public:
		virtual ~BaseComponentHelper() {}
		virtual void RemoveComponent(const EntityHandle& entity) = 0;
		virtual void CopyTo(const EntityHandle& from, const EntityHandle& to) = 0;
	};

	template<typename T, typename Enable = void>
	class BLT_API ComponentHelper;

	template<typename T>
	class BLT_API ComponentHelper<T, typename std::enable_if_t<std::is_copy_constructible_v<T>>> : public BaseComponentHelper
	{
	public:
		void RemoveComponent(const EntityHandle& entity) override
		{
			entity.Remove<T>();
		}

		void CopyTo(const EntityHandle& from, const EntityHandle& to) override
		{
			to.AssignFromCopy<T>(*from.GetComponent<T>());
		}
	};

	template<typename T>
	class BLT_API ComponentHelper<T, typename std::enable_if_t<!std::is_copy_constructible_v<T>>> : public BaseComponentHelper
	{
	public:
		void RemoveComponent(const EntityHandle& entity) override
		{
			entity.Remove<T>();
		}

		void CopyTo(const EntityHandle& from, const EntityHandle& to) override
		{
			BLT_ASSERT(false, "Attempted to copy component with no copy constructor");
		}
	};

	// ==================================================================================================================
	// EVENTS
	// ==================================================================================================================

	struct BLT_API EntityCreated
	{
	public:
		EntityHandle Entity;
	};

	struct BLT_API EntityDestroyed
	{
	public:
		EntityHandle Entity;
	};

	// ==================================================================================================================
	// ENTITY MANAGER
	// ==================================================================================================================

	class BLT_API EntityManager
	{
	public:
		static constexpr uint64_t MAX_COMPONENTS = 64;
		using EntityId = typename Entity::Id_t;
		using ComponentMask = std::bitset<MAX_COMPONENTS>;

	// ==================================================================================================================
	// VIEWS
	// ==================================================================================================================
	public:
		class BLT_API ViewIterator : public std::iterator<std::random_access_iterator_tag, EntityHandle>
		{
		private:
			const EntityManager* m_Manager;
			EntityManager::ComponentMask m_Mask;
			EntityManager::EntityId m_Index;

		public:
			ViewIterator(const EntityManager* manager, EntityManager::ComponentMask componentMask, EntityManager::EntityId index);
			ViewIterator(const EntityManager* manager, EntityManager::EntityId index);

			ViewIterator& operator++();
			EntityHandle operator*() const;
			EntityHandle operator*();

			bool operator==(const ViewIterator& other) const;
			bool operator!=(const ViewIterator& other) const;

		private:
			void Next();
			bool IsValid() const;
		};

		class BLT_API BaseView
		{
		public:
			using Iterator = ViewIterator;

		private:
			const EntityManager* m_Manager;
			EntityManager::ComponentMask m_Mask;

		public:
			BaseView(const EntityManager* manager, EntityManager::ComponentMask componentMask = {});

			Iterator begin();
			Iterator end();
			const Iterator begin() const;
			const Iterator end() const;
		};

		template<typename ... T>
		class BLT_API View : public BaseView
		{
		public:
			View(const EntityManager* manager) : BaseView(manager, manager->GetComponentMask<T...>())
			{
			}
		};

	private:
		EntityId m_IndexCounter;
		std::vector<EntityId> m_FreeList;

		std::vector<ComponentMask> m_ComponentMasks;
		std::vector<std::unique_ptr<BasePool>> m_ComponentPools;
		std::vector<std::unique_ptr<BaseComponentHelper>> m_ComponentHelpers;

		std::unique_ptr<EventBus> m_Bus;
		EventEmitter<EntityCreated> m_OnEntityCreated;
		EventEmitter<EntityDestroyed> m_OnEntityDestroyed;

	public:
		EntityManager();
		~EntityManager();

		EventEmitter<EntityCreated>& OnEntityCreated();
		EventEmitter<EntityDestroyed>& OnEntityDestroyed();

		const EventBus& Bus() const;
		EventBus& Bus();
		size_t EntityCount() const;
		size_t EntityCapacity() const;
		bool IsValid(const Entity& entity) const;

		EntityHandle Get(const Entity& entity) const;

		EntityHandle Create();
		EntityHandle CreateFrom(const EntityHandle& other);
		void Destroy(const Entity& entity);
		void Clear();

		template<typename T, typename ... Args>
		ComponentHandle<T> Assign(const Entity& entity, Args&&... args)
		{
			BLT_ASSERT(ValidEntity(entity), "Invalid Entity");
			if (HasComponent<T>(entity))
			{
				Remove<T>(entity);
			}
			const BaseComponent::Family family = GetComponentFamily<T>();
			Pool<T>* pool = AccomodateComponent<T>();
			// Use placement new to put component into pool
			new(pool->Get(GetEntityIndex(entity))) T(std::forward<Args>(args)...);

			m_ComponentMasks[GetEntityIndex(entity)].set(family);
			return ComponentHandle<T>(this, entity);
		}

		template<typename T>
		void Remove(const Entity& entity)
		{
			BLT_ASSERT(ValidEntity(entity), "Invalid Entity");
			const BaseComponent::Family family = GetComponentFamily<T>();
			BLT_ASSERT(ValidFamily(family), "Invalid Component Type")
			BasePool* pool = m_ComponentPools[family].get();

			m_ComponentMasks[GetEntityIndex(entity)].reset(family);
			pool->Destroy(GetEntityIndex(entity));
		}

		template<typename T>
		bool HasComponent(const Entity& entity) const
		{
			BLT_ASSERT(ValidEntity(entity), "Invalid Entity");
			const BaseComponent::Family family = GetComponentFamily<T>();
			if (!ValidFamily(family))
			{
				return false;
			}
			return m_ComponentMasks[GetEntityIndex(entity)].test(family);
		}

		template<typename T1>
		bool HasComponents(const Entity& entity) const
		{
			return HasComponent<T1>(entity);
		}

		template<typename T1, typename T2, typename ... T>
		bool HasComponents(const Entity& entity) const
		{
			return HasComponent<T1>(entity) && HasComponents<T2, T...>(entity);
		}

		template<typename T, typename = typename std::enable_if<std::is_const<T>::value>::type>
		ComponentHandle<T, const EntityManager> GetComponent(const Entity& entity) const
		{
			BLT_ASSERT(ValidEntity(entity), "Invalid Entity");
			const BaseComponent::Family family = GetComponentFamily<T>();
			if (!ValidFamily(family) || !m_ComponentMasks[GetEntityIndex(entity)].test(family))
			{
				return ComponentHandle<T, const EntityManager>();
			}
			return ComponentHandle<T, const EntityManager>(this, entity);
		}

		template<typename T, typename = typename std::enable_if<!std::is_const<T>::value>::type>
		ComponentHandle<T> GetComponent(const Entity& entity)
		{
			BLT_ASSERT(ValidEntity(entity), "Invalid Entity");
			const BaseComponent::Family family = GetComponentFamily<T>();
			if (!ValidFamily(family) || !m_ComponentMasks[GetEntityIndex(entity)].test(family))
			{
				return ComponentHandle<T>();
			}
			return ComponentHandle<T>(this, entity);
		}

		template<typename ... T>
		std::tuple<ComponentHandle<const T, const EntityManager>...> GetComponents(const Entity& entity) const
		{
			return std::make_tuple(GetComponent<const T>(entity)...);
		}

		template<typename ... T>
		std::tuple<ComponentHandle<T>...> GetComponents(const Entity& entity)
		{
			return std::make_tuple(GetComponent<T>(entity)...);
		}

		inline BaseView GetEntities() const
		{
			return BaseView(this);
		}

		template<typename ... T>
		View<T...> GetEntitiesWith() const
		{
			return View<T...>(this);
		}

	private:
		template<typename T>
		static BaseComponent::Family GetComponentFamily()
		{
			return Component<T>::GetFamily();
		}

	private:
		void DestroyInternal(const Entity& entity);

		template<typename T>
		ComponentMask GetComponentMask() const
		{
			ComponentMask mask;
			const BaseComponent::Family family = GetComponentFamily<T>();
			mask.set(family);
			return mask;
		}

		template<typename T>
		ComponentMask GetComponentMask(const ComponentHandle<T>& component) const
		{
			return GetComponentMask<T>();
		}

		template<typename T1, typename T2, typename ... T>
		ComponentMask GetComponentMask() const
		{
			return GetComponentMask<T1>() | GetComponentMask<T2, T...>();
		}

		template<typename T1, typename ... T>
		ComponentMask GetComponentMask(const ComponentHandle<T1>& component, const ComponentHandle<T>&... components) const
		{
			return GetComponentMask<T1, T...>();
		}

		template<typename T>
		Pool<T>* AccomodateComponent()
		{
			const BaseComponent::Family family = GetComponentFamily<T>();
			if (m_ComponentPools.size() <= family)
			{
				m_ComponentPools.resize(family + 1);
			}
			if (m_ComponentPools[family] == nullptr)
			{
				std::unique_ptr<Pool<T>> pool = std::make_unique<Pool<T>>();
				pool->Expand(m_IndexCounter);
				m_ComponentPools[family] = std::move(pool);
			}
			if (m_ComponentHelpers.size() <= family)
			{
				m_ComponentHelpers.resize(family + 1);
			}
			if (m_ComponentHelpers[family] == nullptr)
			{
				std::unique_ptr<ComponentHelper<T>> helper = std::make_unique<ComponentHelper<T>>();
				m_ComponentHelpers[family] = std::move(helper);
			}
			return (Pool<T>*)m_ComponentPools[family].get();
		}

		void AccomodateEntity(EntityId id);

		size_t GetEntityIndex(const Entity& entity) const;
		bool ValidFamily(const BaseComponent::Family family) const;
		bool ValidEntity(const Entity& entity) const;
		EntityId GetNextEntityId();

		template<typename T>
		T* GetComponentPtr(const Entity& entity)
		{
			BLT_ASSERT(IsValid(entity), "Entity is not valid");
			const BaseComponent::Family family = GetComponentFamily<T>();
			BasePool* pool = m_ComponentPools[family].get();
			BLT_ASSERT(pool != nullptr, "Invalid component pool");
			return (T*)pool->Get(GetEntityIndex(entity));
		}

		template<typename, typename>
		friend class ComponentHandle;

	};

	// ==================================================================================================================
	// ENTITY HANDLE TEMPLATE IMPLEMENTATION
	// ==================================================================================================================

	template<typename T>
	inline bool EntityHandle::HasComponent() const
	{
		BLT_ASSERT(IsValid(), "Entity is not valid");
		return m_Manager->template HasComponent<T>(m_Entity);
	}

	template<typename T1, typename ... T>
	inline bool EntityHandle::HasComponents() const
	{
		BLT_ASSERT(IsValid(), "Entity is not valid");
		return m_Manager->template HasComponents<T1, T...>(m_Entity);
	}

	template<typename T, typename ... Args>
	inline ComponentHandle<T> EntityHandle::Assign(Args&& ... args) const
	{
		BLT_ASSERT(IsValid(), "Entity is not valid");
		return m_Manager->template Assign<T>(m_Entity, std::forward<Args>(args)...);
	}

	template<typename T>
	inline ComponentHandle<T> EntityHandle::AssignFromCopy(const T& other) const
	{
		BLT_ASSERT(IsValid(), "Entity is not Valid");
		return m_Manager->template Assign<T>(m_Entity, std::forward<const T&>(other));
	}

	template<typename T>
	inline void EntityHandle::Remove() const
	{
		BLT_ASSERT(IsValid(), "Entity is not valid");
		return m_Manager->template Remove<T>(m_Entity);
	}

	template<typename T>
	inline ComponentHandle<T> EntityHandle::GetComponent() const
	{
		BLT_ASSERT(IsValid(), "Entity is not valid");
		return m_Manager->template GetComponent<T>(m_Entity);
	}

	template<typename ... T>
	inline std::tuple<ComponentHandle<T>...> EntityHandle::GetComponents() const
	{
		BLT_ASSERT(IsValid(), "Entity is not valid");
		return m_Manager->template GetComponents<T...>(m_Entity);
	}

}