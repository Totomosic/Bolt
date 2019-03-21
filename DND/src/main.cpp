#include "bltpch.h"

#include "Scenes/CharacterScene.h"
#include "Scenes/GameData.h"
#include "Scenes/GameScene.h"
#include "Scenes/ServerScene.h"
#include "Scenes/LoadingScene.h"
#include "Scenes/TitleScene.h"

#include "NetworkManager.h"

namespace DND
{

	class App : public Application
	{
	public:
		void Init() override
		{
			AppWindow->CentreOnMonitor(Monitor::Primary());
			ResourcePack resources = ResourceManager::FetchPack("res/resources.pack");
			ResourceManager::LoadPack(resources);
			ResourceManager::Register(std::make_unique<Font>("res/arial.ttf", 42));

			CreateTilemaps(resources);
			CreateCharacterPrefabs(resources);

			Scene& loadingScene = CreateLoadingScene(resources);
			Scene& titleScene = CreateTitleScene(resources);
			Scene& characterScene = CreateCharacterScene(resources);
			Scene& serverScene = CreateServerScene(resources);
			Scene& gameScene = CreateGameScene(resources);

			SceneManager::SetCurrentScene(titleScene);

			VertexShader vertex;
			ShaderStreamPtr pos = vertex.Position();
			ShaderVariablePtr var = std::make_shared<ShaderVariable>(pos);
			ShaderVariablePtr var0 = std::make_shared<ShaderVariable>(std::make_shared<ShaderLiteral>("vec4(0.0)", ValueType::Vector4f));
			vertex.SetVertexPosition(var);
			vertex.Pass(var);
			CompiledShaderProgram prog = vertex.Compile();
			BLT_INFO(prog.Source);

			NetworkManager::Get().Initialize([](bool initialized)
			{
				
			});			
		}

		void Tick() override
		{
			AppWindow->SetTitle("DND " + std::to_string((int)Time::FramesPerSecond()));
		}

		void Update() override
		{
			NetworkManager::Get().Update();
			if (Input::Get().KeyPressed(Keycode::Esc))
			{
				SceneManager::SetCurrentSceneByName("Title");
			}
			if (Input::Get().KeyPressed(Keycode::R))
			{
				NetworkManager::Get().Reconnect([](bool initialized)
				{

				});
			}
		}

		void Render() override
		{
			Graphics::RenderScene();
		}

		void Exit() override
		{
			NetworkManager::Get().Terminate([this]()
			{
				Application::Exit();
			});
		}

	};

}

int main()
{
	EngineCreateInfo info;
	Engine e(info);
	WindowCreateInfo window;
	window.Title = "DND";
	window.Width = 1280;
	window.Height = 720;
	e.SetWindowCreateInfo(window);
	e.SetApplication<DND::App>();
	e.Run();
	return 0;
}