#include "BoltEngine.h"

using namespace Bolt;

namespace NatureScene
{

	class App : public Application
	{
	public:
		float CAMERA_SPEED = 5;
		float MOUSE_X_SENSITIVITY = 0.005f;
		float MOUSE_Y_SENSITIVITY = 0.005f;
		Camera* perspectiveCamera;

		ObjectFactory Factory;

	public:
		void Init() override
		{
			Scene* scene = SceneManager::CreateScene();
			Layer* layer = scene->CreateLayer<SceneArray>("Main");
			perspectiveCamera = scene->CreateCamera(Frustum::Perspective(PI / 3, PrimaryWindow->GetFramebuffer().Aspect(), 1, 1000), ProjectionType::Perspective);
			layer->SetActiveCamera(perspectiveCamera);

			ResourcePack resources = ResourceManager::FetchPack("res/resources.pack");
			ResourceManager::LoadPack(resources);

			Factory.SetCurrentLayer(layer);
			GameObject* cube = Factory.Cuboid(2, 2, 2, Color::White, Transform({ 0, 0, -10 }));
			Factory.Cuboid(50, 0, 50, Color::Red, Transform({ 0, -1, 0 }));

			id_t rendererId = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().RenderPasses.push_back({ Graphics::DefaultFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(rendererId), {}, {}, {}, { LightSource{ Vector3f(0, 1000, 0) } } });
		}

		void Update() override
		{
			if (Input::KeyDown(Keycode::W))
			{
				perspectiveCamera->transform().Translate(perspectiveCamera->transform().Forward() * CAMERA_SPEED * Time::DeltaTime());
			}
			if (Input::KeyDown(Keycode::S))
			{
				perspectiveCamera->transform().Translate(perspectiveCamera->transform().Forward() * -CAMERA_SPEED * Time::DeltaTime());
			}
			if (Input::KeyDown(Keycode::D))
			{
				perspectiveCamera->transform().Translate(perspectiveCamera->transform().Right() * CAMERA_SPEED * Time::DeltaTime());
			}
			if (Input::KeyDown(Keycode::A))
			{
				perspectiveCamera->transform().Translate(perspectiveCamera->transform().Right() * -CAMERA_SPEED * Time::DeltaTime());
			}
			if (Input::MouseButtonDown(MouseButton::Left))
			{
				float x = Input::RelMousePosition().x * MOUSE_X_SENSITIVITY;
				float y = Input::RelMousePosition().y * MOUSE_Y_SENSITIVITY;
				perspectiveCamera->transform().Rotate(-x, Vector3f::Up(), Space::World);
				perspectiveCamera->transform().Rotate(y, Vector3f::Right(), Space::Local);
			}
		}

		void Render() override
		{
			Graphics::RenderScene();
		}

	};

}

void main()
{
	Engine e;
	WindowCreateInfo info;
	e.SetWindow(std::make_unique<Window>(1280, 720, "Scene", info));
	e.SetApplication(std::make_unique<NatureScene::App>());
	e.Run();
}