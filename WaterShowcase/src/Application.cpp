#include "BoltEngine.h"
#include "PlayerController.h"
#include "TerrainTileManager.h"

using namespace Bolt;

namespace WaterShowcase
{

	class App : public Application
	{
	public:
		TerrainTileManager* tileManager;
		RenderTexture2D* reflectionTexture;
		Camera* defaultCamera;
		GameObject* water;

	public:
		void Init() override
		{
			Window* mainWindow = PrimaryWindow;

			GLState::SetClippingPlane(0, true);

			reflectionTexture = new RenderTexture2D(512, 512);
			reflectionTexture->ClearColor() = Color::SkyBlue;

			Image skyboxFront = LoadTexture("res/front.png", false);
			Image skyboxBack = LoadTexture("res/back.png", false);
			Image skyboxLeft = LoadTexture("res/left.png", false);
			Image skyboxRight = LoadTexture("res/right.png", false);
			Image skyboxTop = LoadTexture("res/top.jpg", false);
			Image skyboxBottom = LoadTexture("res/bottom.jpg", false);
			TextureCube* skyboxTexture = ResourceManager::Register("SkyboxTexture", std::make_unique<TextureCube>(skyboxBack, skyboxFront, skyboxLeft, skyboxRight, skyboxBottom, skyboxTop));

			Image waterDudvImage = LoadTexture("res/waterDUDV.png");
			Image waterNormalImage = LoadTexture("res/normal.png");
			Texture2D* waterDudv = ResourceManager::Register("WaterDUDV", std::make_unique<Texture2D>(waterDudvImage));
			Texture2D* waterNormal = ResourceManager::Register("WaterNormal", std::make_unique<Texture2D>(waterNormalImage));

			Shader* waterShader = ResourceManager::Register("WaterShader", Shader::FromFile("res/Water.shader"));

			Scene* scene = SceneManager::CreateScene();
			Layer* defaultLayer = scene->CreateLayer<SceneArray>("Default");
			Layer* waterLayer = scene->CreateLayer<SceneArray>("Water");

			Mesh waterMesh;
			waterMesh.Models.push_back({ ResourceManager::Register("WaterModel", std::make_unique<Model>(GridFactory(512, 512))), Matrix4f::Scale(10, 1, 10), { 0 } });
			waterMesh.Materials[0].Shader = waterShader;
			waterMesh.Materials[0].Textures.Textures.push_back(reflectionTexture);
			waterMesh.Materials[0].Textures.Textures.push_back(waterDudv);
			waterMesh.Materials[0].Textures.Textures.push_back(waterNormal);
			waterMesh.Materials[0].BaseColor = Color(150, 175, 255, 200);

			Mesh terrainMesh;
			//terrainMesh.Materials[0].BaseColor = Color::LawnGreen * 0.6f;
			terrainMesh.Materials[0].BaseColor.a = 1.0f;

			Mesh cubeMesh;
			cubeMesh.Models.push_back({ ResourceManager::Register("CubeModel", std::make_unique<Model>(CuboidFactory())), Matrix4f::Identity(), { 0 } });

			Mesh skyboxMesh;
			skyboxMesh.Models.push_back({ ResourceManager::Register("SkyboxModel", std::make_unique<Model>(CuboidFactory(512, 512, 512))), Matrix4f::Scale(1), { 0 } });
			skyboxMesh.Materials[0].Textures.Textures.push_back(skyboxTexture);
			skyboxMesh.Materials[0].Shader = Shader::DefaultSkybox();
			skyboxMesh.Materials[0].RenderOptions.UseCullFace = false;

			defaultCamera = scene->CreateCamera();
			defaultLayer->SetActiveCamera(defaultCamera);
			defaultCamera->SetProjection(Projection::Perspective(PI / 3.0f, mainWindow->GetFramebuffer().Aspect(), 1.0f, 5000.0f));
			waterLayer->SetActiveCamera(defaultCamera);

			defaultCamera->transform().SetLocalPosition(0, 3, 0);
			defaultCamera->Components().AddComponent(std::make_unique<PlayerController>(50));

			water = GameObject::Instantiate(waterLayer);
			water->Components().AddComponent(std::make_unique<MeshRenderer>(waterMesh));
			GameObject* cube = GameObject::Instantiate(defaultLayer);
			cube->Components().AddComponent(std::make_unique<MeshRenderer>(cubeMesh));
			cube->transform().SetLocalPosition(0, 10, -10);
			GameObject* skybox = GameObject::Instantiate(defaultLayer);
			skybox->Components().AddComponent(std::make_unique<MeshRenderer>(skyboxMesh));
			GameObject* terrain = GameObject::Instantiate(defaultLayer);
			terrain->Components().AddComponent(std::make_unique<MeshRenderer>(terrainMesh));
			terrain->transform().SetLocalPosition(0, -1, 0);

			SimplexNoise sn(0.5f, 1.0f, 4, 0.25f);
			tileManager = new TerrainTileManager(sn, terrain, 512, 128, 1.0f);

			id_t rendererId = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().RenderPasses.push_back({ reflectionTexture, scene->GetMaskOfLayer("Default"), Graphics::GetRenderer(rendererId) });
			Graphics::Schedule().RenderPasses.push_back({ &PrimaryWindow->GetFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(rendererId) });

			Graphics::Schedule().RenderPasses[0].Uniforms.AddUniform("u_ClippingPlane.Normal", Vector3f::Up(), UniformUploadType::Recurring);
			Graphics::Schedule().RenderPasses[0].Uniforms.AddUniform("u_ClippingPlane.Distance", 0.0f, UniformUploadType::Recurring);

			Graphics::Schedule().RenderPasses[1].Uniforms.AddUniform("u_ClippingPlane.Normal", -Vector3f::Up(), UniformUploadType::Recurring);
			Graphics::Schedule().RenderPasses[1].Uniforms.AddUniform("u_ClippingPlane.Distance", 1000.0f, UniformUploadType::Recurring);
			Graphics::Schedule().RenderPasses[1].Uniforms.AddUniform("u_Lights[0].Position", Vector3f(0, 300, 0), UniformUploadType::Recurring);
			Graphics::Schedule().RenderPasses[1].Uniforms.AddUniform("u_Lights[0].Color", Color::White, UniformUploadType::Recurring);
			Graphics::Schedule().RenderPasses[1].Uniforms.AddUniform("u_Lights[0].Ambient", 0.2f, UniformUploadType::Recurring);
			Graphics::Schedule().RenderPasses[1].Uniforms.AddUniform("material.SpecularHighlight", Color::White, UniformUploadType::Recurring);
			Graphics::Schedule().RenderPasses[1].Uniforms.AddUniform("material.Shininess", 2.0f, UniformUploadType::Recurring);
			Graphics::Schedule().RenderPasses[1].Uniforms.AddUniform("material.ShineDamper", 5.0f, UniformUploadType::Recurring);

			Graphics::Schedule().RenderPasses[1].Uniforms.AddUniform("u_MoveFactor", 0.0f, UniformUploadType::Recurring);
		}

		void Update() override
		{
			std::cout << Time::FramesPerSecond() << std::endl;
			tileManager->Update(defaultCamera->transform().Position());
		}

		void Render() override
		{
			Graphics::Schedule().RenderPasses[0].CameraOverrides[0] = { Matrix4f::InvertY(defaultCamera->ViewMatrix()), defaultCamera->ProjectionMatrix() };
			Graphics::Schedule().RenderPasses[1].Uniforms.GetUniform<float>("u_MoveFactor") += Time::DeltaTime() * 0.05f;

			Graphics::RenderScene();
		}

	};

}

int main()
{
	Engine e;
	std::unique_ptr<Window> mainWindow = std::make_unique<Window>(1280, 720, "Water Showcase", WindowCreateInfo());
	mainWindow->SetClearColor(Color::CornflowerBlue);
	e.SetWindow(std::move(mainWindow));
	e.SetApplication(std::make_unique<WaterShowcase::App>());
	while (!e.ShouldClose())
	{
		e.UpdateApplication();
	}
	return 0;
}