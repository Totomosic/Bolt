#include "Bolt.h"

namespace SlitherLink
{
	
	class App : public Application
	{
	private:
		Camera* camera;

	public:
		void Init() override
		{
			GetWindow().SetClearColor(Color(0, 0, 0, 0));
			Scene& s = SceneManager::Get().CreateScene();
			camera = s.CreateCamera(Projection::Perspective(PI / 3, GetWindow().Aspect(), 0.1f, 5000.0f));
			Layer& l = s.CreateLayer(camera);
			camera->transform().Rotate(-PI / 6, Vector3f::Right());

			ObjectFactory f(l);
			GameObject* object = f.Grid(500, 500, 10, 10, ResourceManager::Get().Materials().PBR(), Transform({ 0, -5, 0 }));
			ShaderLinkContext& material = object->mesh().Mesh.Materials[0]->GetShader();
			material.Link("Albedo", Color::White);
			material.Link("Metallic", 0.1f);
			material.Link("Roughness", 0.8f);
			material.Link("AO", 1.0f);

			LightSource light0;
			light0.Color = Color::White;
			light0.AmbientIntensity = 0.03f;
			light0.Position = { -10, 1000, 10 };
			light0.Intensity = 10;
			LightSource light1;
			light1.Color = Color::White;
			light1.AmbientIntensity = 0;
			light1.Position = { -10, -10, 10 };
			light1.Intensity = 1;
			LightSource light2;
			light2.Color = Color::White;
			light2.AmbientIntensity = 0;
			light2.Position = { 10, -10, 10 };
			light2.Intensity = 1;
			LightSource light3;
			light3.Color = Color::White;
			light3.AmbientIntensity = 0;
			light3.Position = { 10, 10, 10 };
			light3.Intensity = 1;

			ResourceManager::Get().LoadPack("res/resources.pack", [&l](const ResourcePack& resources)
				{
					ObjectFactory f(l);
					ResourcePtr<Texture2D> aoMap = ResourceManager::Get().GetResource<Texture2D>(ResourceManager::Get().Register(std::make_unique<Texture2D>(2, 2)));
					aoMap->LoadPixels();
					for (int i = 0; i < 4; i++)
					{
						aoMap->Pixels[i] = Color::White;
					}
					aoMap->UpdatePixels();
					for (int x = 0; x < 10; x += 2)
					{
						for (int y = 0; y < 10; y += 2)
						{
							GameObject* sphere = f.Sphere(1.0f, ResourceManager::Get().Materials().PBRTexture(), Transform({ (float)x - 4, (float)y - 4, 0 }));
							ShaderLinkContext& material = sphere->mesh().Mesh.Materials[0]->GetShader();
							material.Link("Albedo", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_basecolor")));
							material.Link("Metallic", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_metallic")));
							material.Link("Roughness", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_roughness")));
							material.Link("AO", aoMap);
							sphere->transform().Rotate(x * 2.0f * PI / 8.0f, Vector3f::Up());
							sphere->transform().Rotate(y * 2.0f * PI / 8.0f, Vector3f::Right());
						}
					}
				});
			

			RenderProcess p;
			p.Options.GlobalContext.Lights.push_back(light0);
			p.Options.GlobalContext.Lights.push_back(light1);
			p.Options.GlobalContext.Lights.push_back(light2);
			p.Options.GlobalContext.Lights.push_back(light3);
			RenderSchedule sch(s);
			sch.AddRenderProcess(p);
			SceneRenderer::Get().AddRenderSchedule(sch);
		}

		void Update() override
		{
			Transform& t = camera->transform();
			static float speed = 5;
			if (Input::Get().KeyDown(Keycode::W))
			{
				t.Translate(t.Forward() * speed * Time::Get().RenderingTimeline().DeltaTime());
			}
			if (Input::Get().KeyDown(Keycode::S))
			{
				t.Translate(-t.Forward() * speed * Time::Get().RenderingTimeline().DeltaTime());
			}
			if (Input::Get().KeyDown(Keycode::D))
			{
				t.Translate(t.Right() * speed * Time::Get().RenderingTimeline().DeltaTime());
			}
			if (Input::Get().KeyDown(Keycode::A))
			{
				t.Translate(-t.Right() * speed * Time::Get().RenderingTimeline().DeltaTime());
			}
			if (Input::Get().MouseButtonDown(MouseButton::Middle))
			{
				float x = Input::Get().RelMousePosition().x;
				float y = Input::Get().RelMousePosition().y;
				t.Rotate(-x * 0.003f, Vector3f::Up(), Space::World);
				t.Rotate(y * 0.003f, Vector3f::Right(), Space::Local);
			}
		}

		void Render() override
		{
			Graphics::Get().RenderScene();
		}

	};

}

int main()
{
	EngineCreateInfo e;
	Engine eng(e);
	eng.SetApplication<SlitherLink::App>();
	eng.Run();
}