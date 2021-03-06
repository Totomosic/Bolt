#pragma once
#include "Layer.h"

namespace Bolt
{

	class Scene;

	struct BLT_API SceneLoadEvent
	{
	public:
		Bolt::Scene& Scene;
		std::any Data;
	};

	struct BLT_API SceneUnloadEvent
	{
	public:
		Bolt::Scene& Scene;
		std::any Data;
	};

	class BLT_API Scene
	{
	private:
		std::vector<std::unique_ptr<Layer>> m_Layers;
		bool m_IsLoaded;
		bool m_IsActive;

		EntityManager m_SharedEntities;
		SystemRegistry m_Systems;
		EventEmitter<SceneLoadEvent> m_OnLoad;
		EventEmitter<SceneUnloadEvent> m_OnUnload;
		
	public:
		Scene();

		const EventEmitter<SceneLoadEvent>& OnLoad() const;
		const EventEmitter<SceneUnloadEvent>& OnUnload() const;
		EventEmitter<SceneLoadEvent>& OnLoad();
		EventEmitter<SceneUnloadEvent>& OnUnload();

		const EntityManager& SharedEntities() const;
		EntityManager& SharedEntities();
		const SystemRegistry& Systems() const;
		SystemRegistry& Systems();
		EntityFactory GetFactory();

		Layer& AddLayer();
		void RemoveLayer(Layer& layer);

		bool IsLoaded() const;
		bool IsActive() const;

		void SetIsActive(bool isActive);
		void Clear();

		void Load(const std::any& data = {});
		void Unload(const std::any& data = {});
		void Update(TimeDelta delta);
		void Render(TimeDelta delta);
	};

}