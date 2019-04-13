#pragma once
#include "Scene.h"

namespace Bolt
{

	class BLT_API SceneManager
	{
	private:
		static std::unique_ptr<SceneManager> s_Instance;

	public:
		static SceneManager& Get();

	private:
		std::vector<std::unique_ptr<Scene>> m_Scenes;
		std::unordered_map<blt::string, Scene*> m_SceneMap;
		Scene* m_CurrentScene;

	public:
		SceneManager();

		Scene& CurrentScene();
		Scene& GetSceneById(id_t id);
		Scene& GetSceneByName(const blt::string& name);

		Scene& CreateScene(const blt::string& name = "");
		void SetCurrentScene(Scene& scene);
		void SetCurrentSceneById(id_t id);
		void SetCurrentSceneByName(const blt::string& name);

		template<typename LoadDataT>
		void SetCurrentScene(Scene& scene, LoadDataT data)
		{
			if (m_CurrentScene != nullptr)
			{
				SceneUnloadedEvent e;
				e.UnloadedScene = m_CurrentScene;
				m_CurrentScene->OnUnload.Post(std::move(e));
			}
			m_CurrentScene = &scene;
			if (m_CurrentScene != nullptr)
			{
				std::unique_ptr<LoadDataT> dataPtr = std::make_unique<LoadDataT>();
				*dataPtr = std::move(data);
				SceneLoadedEvent e;
				e.LoadedScene = m_CurrentScene;
				e.LoadData = std::move(dataPtr);
				m_CurrentScene->OnLoad.Post(std::move(e));
			}
		}

		template<typename LoadDataT>
		void SetCurrentSceneById(id_t id, LoadDataT data)
		{
			SetCurrentScene(GetSceneById(id), std::move(data));
		}

		template<typename LoadDataT>
		void SetCurrentSceneByName(const blt::string& name, LoadDataT data)
		{
			SetCurrentScene(GetSceneByName(name), std::move(data));
		}

		friend class Application;
		friend class Destructor;

	private:
		static void Terminate();

	};

}