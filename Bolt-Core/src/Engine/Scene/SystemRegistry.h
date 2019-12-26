#pragma once
#include "Systems/System.h"

namespace Bolt
{

	class BLT_API SystemRegistry
	{
	public:
		template<typename T>
		using SystemPtr = std::shared_ptr<T>;

	private:
		EntityManager& m_Manager;
		std::unordered_map<BaseSystem::Family, SystemPtr<BaseSystem>> m_Systems;
		std::vector<BaseSystem::Family> m_SystemOrder;

	public:
		SystemRegistry(EntityManager& manager);

		int GetSystemCount() const;
		void Configure() const;

		template<typename T>
		bool HasSystem() const
		{
			const BaseSystem::Family family = GetSystemFamily<T>();
			return m_Systems.find(family) != m_Systems.end();
		}

		template<typename T>
		SystemPtr<T> Get() const
		{
			BLT_ASSERT(HasSystem<T>(), "System does not exist");
			const BaseSystem::Family family = GetSystemFamily<T>();
			return m_Systems[family];
		}

		template<typename T>
		SystemPtr<T> AddSystem(const SystemPtr<T>& system, bool addToUpdateOrder = true)
		{
			BLT_ASSERT(!HasSystem<T>(), "System of type already exists");
			const BaseSystem::Family family = GetSystemFamily<T>();
			m_Systems[family] = system;
			if (addToUpdateOrder)
			{
				m_SystemOrder.push_back(family);
			}
			return system;
		}

		template<typename T, typename ... Args>
		SystemPtr<T> Add(Args&&... args)
		{
			return AddSystem<T>(std::make_shared<T>(std::forward<Args>(args)...), true);
		}

		// Used to add a system without it being updated with UpdateAll()
		// Used for adding systems that must be updated manually
		template<typename T, typename ... Args>
		SystemPtr<T> Register(Args&&... args)
		{
			return AddSystem<T>(std::make_shared<T>(std::forward<Args>(args)...), false);
		}

		template<typename T>
		SystemPtr<T> InsertSystem(int index, const SystemPtr<T>& system)
		{
			BLT_ASSERT(!HasSystem<T>(), "System of type already exists");
			const BaseSystem::Family family = GetSystemFamily<T>();
			m_Systems[family] = system;
			m_SystemOrder.insert(m_SystemOrder.begin() + index, system);
			return system;
		}

		template<typename T, typename ... Args>
		SystemPtr<T> Insert(int index, Args&&... args)
		{
			return InsertSystem<T>(index, std::make_shared<T>(std::forward<Args>(args)...));
		}

		template<typename T>
		void Update(TimeDelta delta) const
		{
			BLT_ASSERT(HasSystem<T>(), "System does not exist");
			const BaseSystem::Family family = GetSystemFamily<T>();
			UpdateSystem(*Get<T>(), delta);
		}

		void Update(const SystemPtr<BaseSystem>& system, TimeDelta delta) const;
		void UpdateAll(TimeDelta delta) const;

	private:
		template<typename T>
		static BaseSystem::Family GetSystemFamily()
		{
			return System<T>::GetFamily();
		}

	private:
		void UpdateSystem(BaseSystem& system, TimeDelta delta) const;
	};

}