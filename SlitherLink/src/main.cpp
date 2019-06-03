#include "Bolt.h"
#include "Graphics/Resources/Meshes/Materials/MaterialGraph/PBRMaterialGraph.h"
#include "Graphics/Resources/Meshes/Materials/MaterialGraph/Nodes/Math/MultiplyNode.h"
#include "Graphics/Resources/Meshes/Materials/MaterialGraph/Nodes/Defaults/ValueNodes.h"

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

			PBRMaterialGraph graph;
			MaterialNode& value = graph.AddNode(std::make_unique<FloatNode>(0.2f));
			MaterialNode& mul = graph.AddNode(std::make_unique<MultiplyNode>());
			mul.Connect(0, value.GetConnection(0));
			mul.Connect(1, graph.GetBuilder().GetContext().VertexPosition().GetConnection(0));
			graph.SetVertexPosition(mul.GetConnection(0));
			graph.Build();

			ObjectFactory f(l);
			GameObject* sphere = f.Sphere(2, graph.GetMaterial());
			GameObject* sphere2 = f.Sphere(2, Color::Red, Transform({ 5, 0, 0 }));

			RenderSchedule sch(s);
			sch.AddRenderProcess({});
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