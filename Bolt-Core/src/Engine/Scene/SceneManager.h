#pragma once
#include "Scene.h"

namespace Bolt
{

	class BLT_API SceneManager
	{
	private:
		static std::vector<std::unique_ptr<Scene>> s_Scenes;
		static Scene* s_CurrentScene;
		static Window* s_Window;

	public:
		SceneManager() = delete;

		static Scene& CurrentScene();

		static Scene* CreateScene();
		static void SetCurrentScene(Scene* scene);

		friend class Engine;
		friend struct Scene;
		friend class Application;

	private:
		static void Terminate();

	};

}