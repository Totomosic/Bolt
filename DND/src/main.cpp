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

			ShaderFactory s;
			s.CurrentShader(ShaderType::Vertex);
			ShaderValuePtr position = s.GetStream(ShaderStream::Position);
			ShaderValuePtr texCoord = s.GetStream(ShaderStream::TexCoord);
			ShaderValuePtr modelMatrix = s.RendererUniform(RendererUniform::ModelMatrix);
			ShaderValuePtr viewMatrix = s.RendererUniform(RendererUniform::ViewMatrix);
			ShaderValuePtr projectionMatrix = s.RendererUniform(RendererUniform::ProjectionMatrix);
			ShaderValuePtr worldPosition = s.FuncResult(s.Operations().Mul(), { modelMatrix, position });
			ShaderValuePtr viewPosition = s.FuncResult(s.Operations().Mul(), { viewMatrix, worldPosition });
			ShaderValuePtr screenPosition = s.FuncResult(s.Operations().Mul(), { projectionMatrix, viewPosition });
			s.SetAttribute(ShaderAttribute::Position, screenPosition);
			ShaderValuePtr worldPositionPass = s.Pass(worldPosition);
			ShaderValuePtr texCoordPass = s.Pass(texCoord);

			s.CurrentShader(ShaderType::Fragment);
			ShaderValuePtr texture = s.Uniform("BaseTexture", ValueType::Texture2D);
			s.SetAttribute(ShaderAttribute::FragColor, s.FuncResult(s.Operations().Texture(), { texture, texCoordPass }));

			BLT_INFO(s.VertexSource());
			BLT_INFO(s.FragmentSource());
			ShaderLinkContext link(s);

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