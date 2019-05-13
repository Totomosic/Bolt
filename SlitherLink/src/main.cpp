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
			Scene& s = SceneManager::Get().CreateScene();
			camera = s.CreateCamera(Projection::Perspective(PI / 3, GetWindow().Aspect(), 0.1f, 100.0f));
			Layer& l = s.CreateLayer(camera);
			camera->transform().Rotate(-PI / 6, Vector3f::Right());

			ObjectFactory f(l);
			GameObject* object = f.Grid(50, 50, 50, 50, Color::White, Transform({ 0, -1, 0 }));
			Material& material = *object->mesh().Mesh.Materials[0];
			material.GetShader().Link("Reflectivity", 1.0f);

			LightSource light;
			light.Color = Color::White;
			light.AmbientIntensity = 0.3f;
			light.Position = { 0, 10, 0 };

			RenderProcess p;
			p.Options.GlobalContext.Lights.push_back(light);
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