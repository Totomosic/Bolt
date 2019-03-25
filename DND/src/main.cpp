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
			ShaderVariablePtr worldPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ModelMatrix), vertex.Position()));
			ShaderVariablePtr viewPostition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ViewMatrix), worldPosition));
			ShaderVariablePtr screenPosition = ShaderVariable::Create(ShaderFuncs::Mul(vertex.RendererUniform(RendererUniform::ProjectionMatrix), viewPostition));
			vertex.SetVertexPosition(screenPosition);
			ShaderVariablePtr worldNormal = ShaderVariable::Create(ShaderFuncs::Mul(vertex.Uniform<Matrix3f>(), vertex.Normal()));
			vertex.Pass(worldNormal);

			FragmentShader frag;
			frag.SetFragColor(ShaderFuncs::Add(std::make_shared<ShaderLiteral>("vec4(1.0)", ValueType::Vector4f), frag.Uniform(std::make_shared<ShaderLiteral>("vec4(1.0)", ValueType::Vector4f))));

			CompiledShaderProgram prog = vertex.Compile();
			BLT_INFO(prog.Source);

			CompiledShaderProgram fragProg = frag.Compile();
			BLT_INFO(fragProg.Source);

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