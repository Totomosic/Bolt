#pragma once
#include "Component.h"

namespace Bolt
{

	class GameObject;

	class BLT_API ComponentManager
	{
	public:
		static constexpr int MAX_COMPONENTS = 20;

	private:
		std::unordered_map<size_t, std::unique_ptr<Component>> m_ComponentMap;
		Component* m_ComponentArray[MAX_COMPONENTS];
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
		Component& GetComponent(size_t componentTypeHash) const;
		bool HasComponentById(id_t id) const;
		bool HasComponent(size_t componentTypeHash) const;
		std::vector<Component*> GetComponents() const;

		Component* AddComponent(size_t componentTypeHash, std::unique_ptr<Component>&& component);
		void RemoveComponentById(id_t id);
		void RemoveComponent(size_t componentTypeHash);
		void Clear();

		template<typename T>
		T& GetComponent() const
		{
			return (T&)GetComponent(typeid(T).hash_code());
		}

		template<typename T>
		bool HasComponent() const
		{
			return HasComponent(typeid(T).hash_code());
		}

		template<typename T>
		T* AddComponent(std::unique_ptr<T>&& component)
		{
			return (T*)AddComponent(typeid(T).hash_code(), std::move(component));
		}

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			return AddComponent<T>(std::make_unique<T>(args...));
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

	};

}