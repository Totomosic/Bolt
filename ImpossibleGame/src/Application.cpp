#include "BoltEngine.h"

using namespace Bolt;

namespace ImpossibleGame
{

	class Game : public Application
	{
	public:

	public:
		void Init() override
		{
			PrimaryWindow->SetClearColor(Color(150, 150, 255));

			Scene* scene = SceneManager::CreateScene();
			Layer* mainLayer = scene->CreateLayer<SceneArray>("Main");

			Camera* mainCamera = scene->CreateCamera(Projection::Orthographic(0, PrimaryWindow->Width(), 0, PrimaryWindow->Height(), -100, 100));
			mainLayer->SetActiveCamera(mainCamera);

			Mesh m;
			m.Models.push_back({ ResourceManager::Register("TestModel", std::make_unique<Model>(RectangleFactory(300, 300))) });
			GameObject* testObject = GameObject::Instantiate(mainLayer);
			testObject->Components().AddComponent(std::make_unique<MeshRenderer>(m));
			testObject->transform().SetLocalPosition(640, 360, 0);

			id_t rendererId = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().RenderPasses.push_back({ &PrimaryWindow->GetFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(rendererId) });
		}

		void Tick() override
		{
			
		}

		void Update() override
		{
		
		}

		void Render() override
		{
			Graphics::RenderScene();
		}

	};

}

int main()
{
	Engine e;
	e.SetWindow(std::make_unique<Window>(1280, 720, "Impossible Game"));
	e.SetApplication(std::make_unique<ImpossibleGame::Game>());
	while (!e.ShouldClose())
	{
		e.UpdateApplication();
	}
	return 0;
}