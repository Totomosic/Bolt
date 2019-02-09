#pragma once
#include "Scene.h"

namespace Bolt
{

	class BLT_API SceneManager
	{
	private:
		static std::vector<std::unique_ptr<Scene>> s_Scenes;
		static std::unordered_map<blt::string, Scene*> s_SceneMap;
		static Scene* s_CurrentScene;
		static Window* s_Window;

	public:
		SceneManager() = delete;

		static Scene& CurrentScene();
		static Scene& GetSceneById(id_t id);
		static Scene& GetSceneByName(const blt::string& name);

		static Scene& CreateScene(const blt::string& name = "");
		static void SetCurrentScene(Scene& scene);
		static void SetCurrentSceneById(id_t id);
		static void SetCurrentSceneByName(const blt::string& name);

		template<typename LoadDataT>
		static void SetCurrentScene(Scene& scene, LoadDataT data)
		{
			if (s_CurrentScene != nullptr)
			{
				SceneUnloadedEvent e;
				e.UnloadedScene = s_CurrentScene;
				s_CurrentScene->OnUnload.Post(std::move(e));
			}
			s_CurrentScene = &scene;
			if (s_CurrentScene != nullptr)
			{
				std::unique_ptr<LoadDataT> dataPtr = std::make_unique<LoadDataT>();
				*dataPtr = std::move(data);
				SceneLoadedEvent e;
				e.LoadedScene = s_CurrentScene;
				e.LoadData = std::move(dataPtr);
				s_CurrentScene->OnLoad.Post(std::move(e));
			}
		}

		template<typename LoadDataT>
		static void SetCurrentSceneById(id_t id, LoadDataT data)
		{
			SetCurrentScene(GetSceneById(id), std::move(data));
		}

		template<typename LoadDataT>
		static void SetCurrentSceneByName(const blt::string& name, LoadDataT data)
		{
			SetCurrentScene(GetSceneByName(name), std::move(data));
		}

		friend struct Scene;
		friend class Application;
		friend class Destructor;

	private:
		static void Terminate();

	};

}