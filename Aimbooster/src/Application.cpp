#include "BoltEngine.h"

using namespace Bolt;

#include "Target.h"

namespace Aimbooster
{

	class App : public Application
	{
	public:
		float TARGETS_PER_SECOND = 2;
		float TARGET_SIZE = 50;
		float TARGET_LIFETIME = 5;

		Layer* mainLayer;

		ObjectFactory factory;
		id_t TARGET_PREFAB;
		id_t timer;
		id_t fpsText;

	public:
		void Init() override
		{
			PrimaryWindow->SetClearColor(Color(52, 52, 52));

			Scene* scene = SceneManager::CreateScene();
			mainLayer = scene->CreateLayer<SceneArray>("Main");
			Camera* mainCamera = scene->CreateCamera(PrimaryWindow->GetFramebuffer().ViewFrustum(-100, 100), ProjectionType::Orthographic);
			mainLayer->SetActiveCamera(mainCamera);

			id_t arial = ResourceManager::Register(std::make_unique<Font>("res/arial.ttf", 24));
			ResourcePtr<Font> arialFont = ResourceManager::Get<Font>(arial);

			Mesh targetMesh;
			targetMesh.Models.push_back({ ResourcePtr<Model>(new Model(EllipseFactory(2, 2)), true), Matrix4f::Identity(), { 0 } });
			targetMesh.Materials[0].BaseColor = Color::Orange;
			ObjectPrefab targetPrefab;
			targetPrefab.Components().AddComponent(std::make_unique<MeshRenderer>(targetMesh));
			targetPrefab.Components().AddComponent(std::make_unique<Target>(TARGET_LIFETIME, TARGET_SIZE, factory));

			factory = ObjectFactory(mainLayer);
			TARGET_PREFAB = factory.AddPrefab(std::move(targetPrefab));
			factory.Rectangle(Width(), 60, Color(30, 30, 30), Transform({ Width() / 2, Height() - 30, -10 }));
			timer = mainLayer->UI().AddElement(std::make_unique<Text>("00:00.0", arialFont, Color::White, Transform({ 20, Height() - 30, -5 }), AlignH::Left));
			fpsText = mainLayer->UI().AddElement(std::make_unique<Text>("60 fps", arialFont, Color::White, Transform({ Width() - 100, Height() - 30, -5 }), AlignH::Left));

			Time::CreateTimer(1 / TARGETS_PER_SECOND, std::bind(&App::CreateTarget, this));

			id_t renderer = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().RenderPasses.push_back({ Graphics::DefaultFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(renderer) });
		}

		void Tick() override
		{
			
		}

		void Update() override
		{
			Text* timerText = mainLayer->UI().GetElementById<Text>(timer);
			Text* fpstext = mainLayer->UI().GetElementById<Text>(fpsText);
			timerText->SetText(TimeToString());
			fpstext->SetText(std::to_string((int)Time::FramesPerSecond()) + " fps");
		}

		void Render() override
		{
			Graphics::RenderScene();
		}	

		void CreateTarget()
		{
			GameObject* target = factory.Instantiate(factory.GetPrefab(TARGET_PREFAB), Transform({ Random::NextFloat(0, Width()), Random::NextFloat(0, Height() - 60), -20 }));
		}

		blt::string TimeToString()
		{
			double time = Time::CurrentTime();
			int minutes = (int)(time / 60.0);
			int seconds = (int)(time - 60 * minutes);
			int milliseconds = (int)((time - (minutes * 60 + seconds)) * 1000);
			return std::to_string(minutes / 10) + std::to_string(minutes % 10) + ':' + std::to_string(seconds / 10) + std::to_string(seconds % 10) + '.' + std::to_string(milliseconds / 100);
		}

	};

}

int main()
{
	Engine e;
	WindowCreateInfo createInfo;
	createInfo.Samples = 4;
	e.SetWindow(std::make_unique<Window>(1280, 720, "Aim Booster", createInfo));
	e.SetApplication(std::make_unique<Aimbooster::App>());
	e.Run();
}