#include "Bolt.h"
using namespace Bolt;

class App : public Application
{
public:
	Camera* m_Camera;

	RenderTexture2D* m_Framebuffer;

public:
	void Init() override
	{
		GetWindow().SetClearColor(Color::SkyBlue);
		Scene& scene = SceneManager::Get().CreateScene();
		m_Camera = scene.CreateCamera(Projection::Perspective(PI / 3, GetWindow().Aspect(), 0.1f, 100.0f));
		Layer& layer = scene.CreateLayer(m_Camera);
		Camera* displayCamera = scene.CreateCamera(Projection::Orthographic(0, Width(), 0, Height(), 0, 100));
		Layer& displayLayer = scene.CreateLayer(displayCamera);

		m_Framebuffer = new RenderTexture2D(300, 300, TextureComponent::Color);
		ObjectFactory factory(displayLayer);
		GameObject* display = factory.Image(1280, 720, m_Framebuffer, Transform({ Width() / 2, Height() / 2, -10 }));
		display->mesh().Mesh.Materials[0]->SetIsTransparent(true);

		ResourceManager::Get().LoadPack("res/loadingResources.pack", [&layer](const ResourcePack& pack)
			{
				ResourceExtractor resources(pack);
				ObjectFactory factory(layer);
				GameObject* loadingSymbol = factory.Image(3, 3, resources.GetResourcePtr<Texture2D>("loadingSymbol"), Transform({ 0, 0, -10 }));
				loadingSymbol->mesh().Mesh.Materials[0]->SetIsTransparent(true);
				loadingSymbol->Components().AddComponent<TriggerComponent>(TriggerComponent::TriggerFunc(), [](GameObject* object)
					{
						object->transform().Rotate(2 * PI * Time::Get().RenderingTimeline().DeltaTime(), Vector3f::Forward());
					});
			});

		ResourceManager::Get().LoadPack("res/resources.pack", [&layer, this](const ResourcePack& pack)
			{
				layer.Clear();
				ResourceExtractor resources(pack);
				ObjectFactory factory(layer);
				auto material = ResourceManager::Get().Materials().PBRTexture();
				material->LinkAlbedo(resources.GetResourcePtr<Texture2D>("streaked-metal1-albedo"));
				material->LinkMetallic(resources.GetResourcePtr<Texture2D>("streaked-metal1-metalness"));
				material->LinkRoughness(resources.GetResourcePtr<Texture2D>("streaked-metal1-rough"));
				material->LinkAO(resources.GetResourcePtr<Texture2D>("streaked-metal1-ao"));

				for (int i = -3; i <= 3; i++)
				{
					for (int j = -3; j <= 3; j++)
					{
						auto mat = material->Clone<PBRTextureMaterial>();
						factory.Sphere(1, std::move(mat), Transform({ i * 2.1f, j * 2.1f, -10 }))->transform().Rotate(PI / 2, Vector3f::Right());
					}
				}

				auto mat = ResourceManager::Get().Materials().PBR();
				mat->LinkRoughness([]()
					{
						return Map<float>(sin(Time::Get().RenderingTimeline().CurrentTime()), -1, 1, 0.2f, 1.0f);
					});
				factory.Sphere(2, std::move(mat), Transform({ 0, 0, -20 }));
			});

		LightSource sun;
		sun.Position = Vector3f(300, 0, 1000);
		sun.Intensity = 15;
		sun.Color = Color(230, 100, 100);
		sun.Attenuation = Vector3f(1, 0, 0);
		sun.AmbientIntensity = 0.03f;

		RenderProcess process;
		process.Options.GlobalContext.Lights.push_back(sun);
		sun.Position = Vector3f(-300, 0, 1000);
		sun.Color = Color(100, 100, 230);
		process.Options.GlobalContext.Lights.push_back(sun);
		process.Options.RenderTarget = m_Framebuffer;
		process.LayerMask = scene.GetMaskOfLayer(layer.Id());

		RenderProcess displayProcess;
		displayProcess.LayerMask = scene.GetMaskOfLayer(displayLayer.Id());

		RenderSchedule sch(scene);
		sch.AddRenderProcess(process);
		sch.AddRenderProcess(displayProcess);
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
		m_Framebuffer->Clear();
		Graphics::Get().RenderScene();
	}
};

int main()
{
	EngineCreateInfo info;
	info.WindowInfo.Samples = 0;
	Engine e(info);
	e.SetApplication<App>();
	e.Run();
	return 0;
}