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

		friend struct Scene;
		friend class Application;
		friend class Destructor;

	private:
		static void Terminate();

	};

}