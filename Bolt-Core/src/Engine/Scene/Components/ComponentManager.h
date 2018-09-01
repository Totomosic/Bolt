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
		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_ComponentMap;
		Component* m_ComponentArray[MAX_COMPONENTS];
		ObjectPrefab* m_GameObject;

	public:
		ComponentManager(ObjectPrefab* gameObject);
		ComponentManager(const ComponentManager& other) = delete;
		ComponentManager& operator=(const ComponentManager& other) = delete;
		ComponentManager(ComponentManager&& other);
		ComponentManager& operator=(ComponentManager&& other);
		~ComponentManager();

		Component& GetComponent(id_t id) const;
		Component& GetComponent(const std::type_index& componentType) const;
		bool HasComponent(id_t id) const;
		bool HasComponent(const std::type_index& componentType) const;
		std::vector<Component*> GetComponents() const;

		Component* AddComponent(const std::type_index& componentType, std::unique_ptr<Component>&& component);
		void RemoveComponent(id_t id);
		void RemoveComponent(const std::type_index& componentType);

		template<typename T>
		T& GetComponent() const
		{
			return (T&)GetComponent(typeid(T));
		}

		template<typename T>
		bool HasComponent() const
		{
			return HasComponent(typeid(T));
		}

		template<typename T>
		T* AddComponent(std::unique_ptr<T>&& component)
		{
			return (T*)AddComponent(typeid(T), std::move(component));
		}

		template<typename T>
		void RemoveComponent()
		{
			return RemoveComponent(typeid(T));
		}

		friend class GameObject;
		friend class ObjectPrefab;

	private:
		id_t FindNextId() const;
		void SetGameObject(ObjectPrefab* object);

	};

}