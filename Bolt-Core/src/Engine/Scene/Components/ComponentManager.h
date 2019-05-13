#pragma once
#include "Component.h"
#include "Core/Events/IdManager.h"

namespace Bolt
{

	class GameObject;

	class BLT_API ComponentManager
	{
	public:
		constexpr static int MAX_COMPONENTS = 20;

		struct BLT_API ComponentInfo
		{
		public:
			std::unique_ptr<Component> component;
			uint32_t type_hash;
		};

		struct BLT_API ComponentInfoPtr
		{
		public:
			Component* component;
			uint32_t type_hash;
		};

	private:
		IdManager<id_t> m_ComponentIdManager;
		ComponentInfo m_ComponentArray[MAX_COMPONENTS];
		std::unordered_map<uint32_t, id_t> m_TypeHashMap;
		std::vector<id_t> m_OrderedIndex;
		ObjectPrefab* m_GameObject;
		bool m_IsGameObject;

	public:
		ComponentManager(ObjectPrefab* gameObject);
		ComponentManager(const ComponentManager& other) = delete;
		ComponentManager& operator=(const ComponentManager& other) = delete;
		ComponentManager(ComponentManager&& other);
		ComponentManager& operator=(ComponentManager&& other);
		~ComponentManager();

		Component& GetComponentById(id_t id) const;
		Component& GetComponent(uint32_t componentTypeHash) const;
		// Returns true if component type exists and returns component through outComponent
		bool GetComponent(uint32_t componentTypeHash, Component** outComponent) const;
		bool HasComponentById(id_t id) const;
		bool HasComponent(uint32_t componentTypeHash) const;
		std::vector<Component*> GetComponents() const;
		std::vector<ComponentInfoPtr> GetComponentsOrdered() const;

		Component* AddComponent(uint32_t componentTypeHash, std::unique_ptr<Component>&& component);
		void RemoveComponentById(id_t id);
		void RemoveComponent(uint32_t componentTypeHash);
		void Clear();

		template<typename T>
		T& GetComponent() const
		{
			return (T&)GetComponent(typeid(T).hash_code());
		}

		template<typename T>
		bool GetComponent(T** outComponent)
		{
			return GetComponent(typeid(T).hash_code(), (Component**)outComponent);
		}

		template<typename T>
		bool HasComponent() const
		{
			return HasComponent(typeid(T).hash_code());
		}

		template<typename T>
		T& AddComponent(std::unique_ptr<T>&& component)
		{
			return *(T*)AddComponent(typeid(T).hash_code(), std::move(component));
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return AddComponent<T>(std::make_unique<T>(std::forward<Args>(args)...));
		}

		template<typename T>
		void RemoveComponent()
		{
			return RemoveComponent(typeid(T).hash_code());
		}

		void Transfer(XMLserializer& backend, bool isWriting);

		friend class GameObject;
		friend class ObjectPrefab;

	private:
		id_t FindNextId() const;
		void SetGameObject(ObjectPrefab* object);
		bool HasComponentPrivate(uint32_t componentHasType) const;

	};

}