#include "Bolt.h"
using namespace Bolt;

class App : public Application
{
public:
	Camera* m_Camera;

public:
	void Init() override
	{
		Scene& scene = SceneManager::Get().CreateScene();
		m_Camera = scene.CreateCamera(Projection::Perspective(PI / 3, GetWindow().Aspect(), 0.1f, 100.0f));
		Layer& layer = scene.CreateLayer(m_Camera);

		ResourceManager::Get().LoadPack("res/resources.pack", [&layer](const ResourcePack& pack)
			{
				ResourceExtractor resources(pack);
				ObjectFactory factory(layer);
				auto material = ResourceManager::Get().Materials().PBRTexture();
				material->LinkAlbedo(resources.GetResourcePtr<Texture2D>("streaked-metal1-albedo"));
				material->LinkMetallic(resources.GetResourcePtr<Texture2D>("streaked-metal1-metalness"));
				material->LinkRoughness(resources.GetResourcePtr<Texture2D>("streaked-metal1-rough"));
				material->LinkAO(resources.GetResourcePtr<Texture2D>("streaked-metal1-ao"));

				auto mat = ResourceManager::Get().Materials().PBR();

				for (int i = -3; i <= 3; i++)
				{
					for (int j = -3; j <= 3; j++)
					{
						factory.Sphere(1, material->Clone(), Transform({ i * 2.1f, j * 2.1f, -10 }))->transform().Rotate(PI / 2, Vector3f::Right());
					}
				}	
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