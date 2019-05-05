#include "Bolt.h"
#include "Constants.h"

#include "PlayerMovement.h"
#include "GameManager.h"
#include "MeteorController.h"

namespace Meteors
{

	float CharacterWidth = 610.0f / 8;
	float CharacterHeight = 810.0f / 8;
	float PlayerSpeed = 800;
	float Gravity = 8000;

	id_t RightFacingCharacterTexture = 0;
	id_t LeftFacingCharacterTexture = 0;
	id_t RollingHillsTexture = 0;
	id_t GroundTexture = 0;
	id_t RegularMeteorTexture = 0;

	class App : public Application
	{
	private:
		Scene* m_GameScene;
		Scene* m_EndScene;

		float m_Time;
		Text* m_TimerText;

		Text* m_FinalScoreText;
		
	public:
		void Init() override
		{
			GetWindow().SetClearColor(Color::White);
			GetWindow().EnableVSync();
			m_GameScene = &SceneManager::Get().CreateScene();
			Camera* gameCamera = m_GameScene->CreateCamera(Projection::Orthographic(0, ViewWidth, 0, ViewHeight, -100, 100));
			m_GameScene->CreateLayer(gameCamera);

			m_EndScene = &SceneManager::Get().CreateScene();
			Camera* endCamera = m_EndScene->CreateCamera(Projection::Orthographic(0, ViewWidth, 0, ViewHeight, -100, 100));
			Layer& endLayer = m_EndScene->CreateLayer(endCamera);
			endLayer.UI().Text("Game Over", ResourceManager::Get().Fonts().Verdana(48), Color::Black, Transform({ ViewWidth / 2, ViewHeight / 2, 0 }));
			m_FinalScoreText = &endLayer.UI().Text("Your final survival time was: ", ResourceManager::Get().Fonts().Verdana(30), Color::Black, Transform({ ViewWidth / 2, ViewHeight / 2 - 24 - 15, 0 }));

			ResourcePack resources = ResourceManager::Get().FetchPack("res/resources.pack");
			ResourceManager::Get().LoadPack(resources);

			RightFacingCharacterTexture = resources.GetResourceId("MarioRight");
			LeftFacingCharacterTexture = resources.GetResourceId("MarioLeft");
			RollingHillsTexture = resources.GetResourceId("RollingHills");
			GroundTexture = resources.GetResourceId("Ground");
			RegularMeteorTexture = resources.GetResourceId("Meteor");

			LoadGameScene();

			RenderSchedule sch(*m_GameScene);
			sch.AddRenderProcess({});
			SceneRenderer::Get().AddRenderSchedule(sch);

			RenderSchedule esch(*m_EndScene);
			esch.AddRenderProcess({});
			SceneRenderer::Get().AddRenderSchedule(esch);
		}

		void LoadGameScene()
		{
			m_Time = 0;
			SceneManager::Get().SetCurrentScene(*m_GameScene);
			Layer& layer = m_GameScene->GetLayer(0);
			layer.Clear();
			GameObject* gameManager = GameObject::Instantiate(&layer);
			GameManager& manager = gameManager->Components().AddComponent<GameManager>();
			ObjectFactory& factory = manager.Factory;
			factory.SetCurrentLayer(layer);
			factory.Image(ViewWidth, ViewHeight, ResourceManager::Get().GetResource<Texture2D>(RollingHillsTexture), Transform({ ViewWidth / 2, ViewHeight / 2, -99 }));
			GameObject * player = factory.Image(CharacterWidth, CharacterHeight, ResourceManager::Get().GetResource<Texture2D>(RightFacingCharacterTexture), Transform({ ViewWidth / 2, FloorHeight + CharacterHeight / 2, 0 }));
			player->Components().AddComponent<PlayerMovement>(PlayerSpeed, 2400, Gravity, 3);

			GameObject* ground = factory.Image(ViewWidth, FloorHeight, ResourceManager::Get().GetResource<Texture2D>(GroundTexture), Transform({ ViewWidth / 2, FloorHeight / 2, -98 }));
			ground->mesh().Mesh.Materials[0]->GetShader().Link("TexCoordMatrix", Matrix3f::Scale(19.2f, 1.0f, 1.0f));

			manager.Player = player;

			m_TimerText = &layer.UI().Text("0:00", Color::Black, Transform({ 10, ViewHeight - 10, 99 }), AlignH::Left, AlignV::Top);
		}

		void LoadEndScene()
		{
			SceneManager::Get().SetCurrentScene(*m_EndScene);
			m_FinalScoreText->SetText("Your final survival time was: " + GetTimeString());
		}

		void Update() override
		{
			m_Time += Time::Get().RenderingTimeline().DeltaTime();
			m_TimerText->SetText(GetTimeString());

			if (Input::Get().KeyPressed(Keycode::R))
			{
				LoadGameScene();
			}
			if (Input::Get().KeyPressed(Keycode::E))
			{
				LoadEndScene();
			}
		}

		void Render() override
		{
			Graphics::Get().RenderScene();
		}

		void Exit() override
		{
			Application::Exit();
		}

		blt::string GetTimeString() const
		{
			blt::string result = "";
			result += std::to_string((int)(m_Time / 60)) + ':';
			result += std::to_string((int)(m_Time) % 60 / 10);
			result += std::to_string((int)(m_Time) % 10);
			return result;
		}

	};

}

int main()
{
	EngineCreateInfo i;
	i.WindowInfo.Title = "Meteors";
	i.WindowInfo.Width = 1280;
	i.WindowInfo.Height = 720;
	Engine e(i);
	e.SetApplication<Meteors::App>();
	e.Run();
}