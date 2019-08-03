#include "Bolt.h"
using namespace Bolt;

class App : public Application
{
public:
	Camera* m_Camera;

public:
	void Init() override
	{
		GetWindow().SetClearColor(Color::SkyBlue);
		Scene& scene = SceneManager::Get().CreateScene();
		m_Camera = scene.CreateCamera(Projection::Perspective(PI / 3, GetWindow().Aspect(), 0.1f, 100.0f));
		Layer& layer = scene.CreateLayer(m_Camera);

		Camera* uiCamera = scene.CreateCamera(Projection::Orthographic(0, Width(), 0, Height(), -100, 100));
		Layer& uiLayer = scene.CreateLayer(uiCamera);

		EventBus test;
		test.AddEventListener<void>(0, [](Event<void>& e) {}, ListenerPriority::Low);
		test.AddEventListener<void>(0, [](Event<void>& e) {}, ListenerPriority::Low);
		test.AddEventListener<void>(0, [](Event<void>& e) {}, ListenerPriority::High);
		test.AddEventListener<void>(0, [](Event<void>& e) {}, ListenerPriority::Medium);
		test.AddEventListener<void>(0, [](Event<void>& e) {}, ListenerPriority::High);
		test.AddEventListener<void>(0, [](Event<void>& e) {}, ListenerPriority::Medium);
		uint32_t id = test.AddEventListener<void>(0, [](Event<void>& e) {}, ListenerPriority::Low);
		test.SetListenerPriorityIndex(id, 0);

		GetWindow().OnResize().AddEventListener([this](Event<WindowResizeEvent>& e)
			{
				m_Camera->SetProjection(Projection::Perspective(PI / 3, GetWindow().Aspect(), 0.1f, 100.0f));
			});

		ResourceManager::Get().LoadPack("res/loadingResources.pack", [&uiLayer, this](const ResourcePack& pack)
			{
				ResourceExtractor resources(pack);
				UIsurface& loadingSymbol = uiLayer.UI().Image(200, 200, resources.GetResourcePtr<Texture2D>("loadingSymbol"), Transform({ Width() / 2, Height() / 2, -10 }));
				loadingSymbol.Object()->mesh().Mesh.Materials[0]->SetIsTransparent(true);
				loadingSymbol.Object()->Components().AddComponent<TriggerComponent>(TriggerComponent::TriggerFunc(), [](GameObject* object)
					{
						object->transform().Rotate(2 * PI * Time::Get().RenderingTimeline().DeltaTime(), Vector3f::Forward());
					});
			});

		ResourceManager::Get().LoadPack("res/resources.pack", [&layer, &uiLayer, this](const ResourcePack& pack)
			{
				uiLayer.Clear();
				ResourceExtractor resources(pack);
				ObjectFactory factory(layer);
				auto material = ResourceManager::Get().Materials().PBRTexture();
				material->LinkAlbedo(resources.GetResourcePtr<Texture2D>("d"));
				material->LinkMetallic(resources.GetResourcePtr<Texture2D>("m"));
				material->LinkRoughness(resources.GetResourcePtr<Texture2D>("r"));
				material->LinkAO(resources.GetResourcePtr<Texture2D>("h"));
				material->LinkNormal(resources.GetResourcePtr<Texture2D>("n"));

				for (int i = -3; i <= 3; i++)
				{
					for (int j = -3; j <= 3; j++)
					{
						auto mat = material->Clone<PBRTextureMaterial>();
						factory.Sphere(1, std::move(mat), Transform({ i * 2.1f, j * 2.1f, -10 }))->transform().Rotate(PI / 2, Vector3f::Right());
					}
				}
			});

		LightSource sun;
		sun.Position = Vector3f(0, 0, 1000);
		sun.Intensity = 10;
		sun.Color = Color::White;
		sun.Attenuation = Vector3f(1, 0, 0);
		sun.AmbientIntensity = 0.1f;

		RenderProcess process;
		process.Options.GlobalContext.Lights.push_back(sun);

		RenderSchedule sch(scene);
		sch.AddRenderProcess(process);
		SceneRenderer::Get().AddRenderSchedule(sch);
	}

	void Update() override
	{
		float speed = 4 * Time::Get().RenderingTimeline().DeltaTime();
		if (Input::Get().KeyDown(Keycode::W))
		{
			m_Camera->transform().Translate(m_Camera->transform().Forward() * speed);
		}
		if (Input::Get().KeyDown(Keycode::S))
		{
			m_Camera->transform().Translate(m_Camera->transform().Forward() * -speed);
		}
		if (Input::Get().KeyDown(Keycode::D))
		{
			m_Camera->transform().Translate(m_Camera->transform().Right() * speed);
		}
		if (Input::Get().KeyDown(Keycode::A))
		{
			m_Camera->transform().Translate(m_Camera->transform().Right() * -speed);
		}
		if (Input::Get().MouseButtonDown(MouseButton::Middle))
		{
			float dx = Input::Get().RelMousePosition().x;
			float dy = Input::Get().RelMousePosition().y;
			m_Camera->transform().Rotate(-dx * 0.003f, Vector3f::Up(), Space::World);
			m_Camera->transform().Rotate(dy * 0.003f, Vector3f::Right(), Space::Local);
		}
	}

	void Render() override
	{
		Graphics::Get().RenderScene();
	}
};

int main()
{
	EngineCreateInfo info;
	Engine e(info);
	e.SetApplication<App>();
	e.Run();
	return 0;
}