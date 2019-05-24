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
			GameObject* object = f.Grid(500, 500, 10, 10, Color::White, Transform({ 0, -5, 0 }));
			LightSource light;
			light.Color = Color::White;
			light.AmbientIntensity = 0.1f;
			light.Position = { 0, 100, 0 };

			RenderProcess p;
			p.Options.GlobalContext.Lights.push_back(light);
			RenderSchedule sch(s);
			sch.AddRenderProcess(p);
			SceneRenderer::Get().AddRenderSchedule(sch);
		}

		void Update() override
		{
			Transform& t = camera->transform();
			static float speed = 50;
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