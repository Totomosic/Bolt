#include "BoltEngine.h"
#include "Meteor.h"
#include "RigidBody.h"
#include "PlayerController.h"

namespace MeteorGame
{

	class App : public Application
	{
	public:
		Camera * orthoCamera;
		GameObject* player;
		Timer* meteorTimer;

	public:
		void Init() override
		{
			Window* mainWindow = PrimaryWindow;
			mainWindow->SetClearColor(Color::CornflowerBlue);
			mainWindow->CentreOnMonitor(Monitor::Primary());

			Image backgroundImage = LoadTexture("res/background.jpg");
			Texture2D* backgroundTexture = ResourceManager::Register("BackgroundTexture", std::make_unique<Texture2D>(backgroundImage));

			Image meteorImage = LoadTexture("res/meteor_texture.jpg");
			Texture2D* meteorTexture = ResourceManager::Register("MTexture0", std::make_unique<Texture2D>(meteorImage));

			Image meteorImage1 = LoadTexture("res/meteor_texture_1.jpg");
			Texture2D* meteorTexture1 = ResourceManager::Register("MTexture1", std::make_unique<Texture2D>(meteorImage1));

			Image explosionImage = LoadTexture("res/explosion.png");
			Texture2D* explosionSheet = ResourceManager::Register("Explosion", std::make_unique<Texture2D>(explosionImage));

			Image explosion2Image = LoadTexture("res/explosion2.png");
			Texture2D* explosion2Sheet = ResourceManager::Register("Explosion2", std::make_unique<Texture2D>(explosion2Image));

			Font* arial = ResourceManager::Register("Arial", std::make_unique<Font>("arial.ttf", 24));

			ResourceManager::Register("Circle", std::make_unique<Model>(EllipseFactory(2, 2)));
			ResourceManager::Register("Square", std::make_unique<Model>(RectangleFactory()));

			Mesh backgroundMesh;
			backgroundMesh.Models.push_back({ ResourceManager::Register("BackgroundModel", std::make_unique<Model>(RectangleFactory())), Matrix4f::Scale(mainWindow->Width(), mainWindow->Height(), 1), { 0 } });
			backgroundMesh.Materials[0].Textures.Textures.push_back(backgroundTexture);
			backgroundMesh.Materials[0].Shader = Shader::DefaultTexture();

			Mesh floorMesh;
			floorMesh.Models.push_back({ ResourceManager::Register("FloorModel", std::make_unique<Model>(RectangleFactory())), Matrix4f::Scale(mainWindow->Width(), 100, 1), { 0 } });
			floorMesh.Materials[0].BaseColor = Color::Black;

			Mesh playerMesh;
			playerMesh.Models.push_back({ ResourceManager::Register("PlayerModel", std::make_unique<Model>(RectangleFactory())), Matrix4f::Scale(50, 150, 1), { 0 } });

			Scene* scene = SceneManager::CreateScene();
			Layer* mainLayer = scene->CreateLayer<SceneArray>("2D");

			orthoCamera = mainLayer->AddCamera(Camera());
			orthoCamera->SetProjection(Projection::Orthographic(0, mainWindow->Width(), 0, mainWindow->Height(), 1.0f, 1000.0f));
			orthoCamera->transform().SetLocalPosition(0, 0, 50);

			GameObject* background = GameObject::Instantiate(mainLayer);
			background->transform().SetLocalPosition(mainWindow->Width() / 2, mainWindow->Height() / 2, -100);
			background->Components().AddComponent(std::make_unique<MeshRenderer>(backgroundMesh));

			GameObject* floor = GameObject::Instantiate(mainLayer);
			floor->transform().SetLocalPosition(mainWindow->Width() / 2, 50, -20);
			floor->Components().AddComponent(std::make_unique<MeshRenderer>(floorMesh));

			player = GameObject::Instantiate(mainLayer);
			player->transform().SetLocalPosition(mainWindow->Width() / 2, 175, -20);
			player->Components().AddComponent(std::make_unique<MeshRenderer>(playerMesh));
			player->Components().AddComponent(std::make_unique<RigidBody>());
			player->Components().AddComponent(std::make_unique<PlayerController>());

			meteorTimer = Time::CreateTimer(1, std::bind(&App::CreateMeteor, this));

			id_t rendererId = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().RenderPasses.push_back({ &PrimaryWindow->GetFramebuffer(), scene->GetMaskOfLayer("2D"), Graphics::GetRenderer(rendererId) });
		}

		void Update() override
		{
			if (Input::KeyDown(Keycode::R))
			{
				SceneManager::CurrentScene().GetLayer(0).Clear();
			}
		}

		void ImGuiRender() override
		{
			//ImGui::ColorEdit4("Color", &SceneManager::CurrentScene().GetLayer(0).GameObjects().GetGameObjectById(1).Components().GetComponent<MeshRenderer>().Mesh.Materials[0].BaseColor.r);
		}

		void Render() override
		{
			float width = orthoCamera->CameraProjection().ViewFrustum.Width();
			float height = orthoCamera->CameraProjection().ViewFrustum.Height();
			Graphics::Text(std::to_string((int)Time::FramesPerSecond()) + " fps", ResourceManager::Get<Font>("Arial"), width - 50, height - 25, 45, Color::Black, AlignH::Center, 0);
			//Graphics::Image(width / 2, height / 2, 40, 300, 300, ResourceManager::Get<Texture2D>("Explosion2"));
			Graphics::RenderScene();
		}

		void CreateMeteor()
		{
			float side = Random::NextFloat(-1.5f, 1.5f);
			float x = Map<float>(side, -1.0f, 1.0f, 0.0f, orthoCamera->CameraProjection().ViewFrustum.Width());
			float y = orthoCamera->CameraProjection().ViewFrustum.Height() + 500;
			float r = Random::NextFloat(25, 75);

			int type = Random::NextInt(0, 1);
			Model* model = ResourceManager::Get<Model>("Circle");

			Mesh mesh;
			mesh.Models.push_back({ model, Matrix4f::Scale(1, 1, 1), { 0 } });
			mesh.Materials[0].Shader = Shader::DefaultTexture();
			mesh.Materials[0].Textures.Textures.push_back(ResourceManager::Get<Texture2D>("MTexture" + std::to_string(type)));

			GameObject* meteor = GameObject::Instantiate(&SceneManager::CurrentScene().GetLayer(0));
			meteor->transform().SetLocalPosition(x, y, -5);
			meteor->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
			meteor->Components().AddComponent(std::make_unique<RigidBody>());
			meteor->Components().AddComponent(std::make_unique<Meteor>(r, (MeteorType)type));
			meteor->transform().SetLocalScale(r, r, 1);
			RigidBody& rb = meteor->Components().GetComponent<RigidBody>();
			rb.Acceleration.y = -300;
			rb.Velocity = Vector3f(Random::NextFloat(200, 700) * -side, Random::NextFloat(-50, 200), 0);
			rb.zAngle = -rb.Velocity.x * 0.03f;
		}

	};

}

int main()
{
	Engine e;
	WindowCreateInfo info;
	info.Samples = 4;
	std::unique_ptr<Window> mainWindow = std::make_unique<Window>(1280, 720, "Test Window", info);
	e.SetWindow(std::move(mainWindow));
	e.SetApplication(std::make_unique<MeteorGame::App>());
	
	while (!e.ShouldClose())
	{
		e.UpdateApplication();
	}

	return 0;
}