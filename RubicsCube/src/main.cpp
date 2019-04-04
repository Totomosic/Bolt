#include "Bolt.h"

namespace RubicsCube
{

	class App : public Application
	{
	public:
		void Init() override
		{
			Scene& scene = SceneManager::CreateScene();
			Camera* camera = scene.CreateCamera(Projection::Perspective(PI / 3.0f, AppWindow->Aspect(), 0.1f, 1000.0f));
			camera->transform().Translate(0, 0, 15);
			Layer& layer = scene.CreateLayer(camera);

			ObjectFactory factory(layer);
			GameObject* cuboid = factory.Cuboid(5, 5, 5, Color::White);
			cuboid->Components().AddComponent<TriggerComponent>(TriggerComponent::TriggerFunc(), [](GameObject* object)
				{
					//object->transform().Rotate(PI / 4 * Time::RenderingTimeline().DeltaTime(), Vector3f(-5, 1, 3).Normalize());
				});
			const Model& model = *cuboid->Components().GetComponent<MeshRenderer>().Mesh.Models[0].Model;
			{
				ModelMapping mapping = model.Map();
				for (Face<3>& face : model.GetFaces<3>(mapping))
				{
					face.Vertices[0][0] += Vector3f(0.5f, 0.5f, 0.5f);
					face.Vertices[1][0] += Vector3f(0.5f, 0.5f, 0.5f);
					face.Vertices[2][0] += Vector3f(0.5f, 0.5f, 0.5f);					
				}
			}
			{
				ModelMapping mapping = model.Map();
				for (Face<3>& face : model.GetFaces<3>(mapping))
				{
					BLT_INFO(face.Vertices[0][0].Read<Vector3f>());
					BLT_INFO(face.Vertices[1][0].Read<Vector3f>());
					BLT_INFO(face.Vertices[2][0].Read<Vector3f>());
				}
			}
			

			RenderSchedule s(scene);
			s.AddRenderProcess({ });
			SceneRenderer::AddRenderSchedule(s);
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