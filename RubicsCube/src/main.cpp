#include "Bolt.h"

#include "RubicsCube.h"

namespace RubicsCube
{

	class App : public Application
	{
	public:
		RubicsCube cube;

	public:
		void Init() override
		{
			Scene& scene = SceneManager::CreateScene();
			Camera* camera = scene.CreateCamera(Projection::Perspective(PI / 3.0f, AppWindow->Aspect(), 0.1f, 1000.0f));
			camera->transform().Translate(0, 0, 10);
			Layer& layer = scene.CreateLayer(camera);

			ObjectFactory factory(layer);
			cube = RubicsCube(factory);

			RenderSchedule s(scene);
			s.AddRenderProcess({ });
			SceneRenderer::AddRenderSchedule(s);
		}

		void Update() override
		{
			if (Input::Get().MouseButtonDown(MouseButton::Middle))
			{
				float x = Input::Get().RelMousePosition().x;
				float y = Input::Get().RelMousePosition().y;
				Transform& t = cube.Root()->transform();
				t.Rotate(-y * 0.01f, Vector3f::Right(), Space::World);
				t.Rotate(x * 0.01f, Vector3f::Up(), Space::World);
			}
		}

		void Render() override
		{
			Graphics::RenderScene();
		}

	};

}

int main()
{
	EngineCreateInfo eInfo;
	WindowCreateInfo wInfo;
	wInfo.Title = "Rubics Cube";
	wInfo.Width = 1280;
	wInfo.Height = 720;
	Engine e(eInfo);
	e.SetWindowCreateInfo(wInfo);
	e.SetApplication<RubicsCube::App>();
	e.Run();
	return 0;
}