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

			ResourceManager::Get().LoadPack("res/resources.pack", [&l](const ResourcePack& resources)
				{
					PBRMaterialGraph graph;
					PropertyNode& albedoProperty = graph.AddProperty("Albedo", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_basecolor")));
					SampleTextureNode& albedoTexture = (SampleTextureNode&)graph.AddNode(std::make_unique<SampleTextureNode>());
					albedoTexture.SetTexture(albedoProperty.GetConnection(0));
					MaterialNode& albedoSplit = graph.AddNode(std::make_unique<SplitNode>());
					albedoSplit.Connect(0, albedoTexture.GetColor());
					graph.SetAlbedo(albedoSplit.GetConnection(0));

					PropertyNode& metallicProperty = graph.AddProperty("Metallic", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_metallic")));
					SampleTextureNode& metallicTexture = (SampleTextureNode&)graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
					metallicTexture.SetTexture(metallicProperty.GetConnection(0));
					graph.SetMetallic(metallicTexture.GetR());

					PropertyNode& aoProperty = graph.AddProperty("AO", ResourceManager::Get().Textures().DefaultWhite());
					SampleTextureNode& aoTexture = (SampleTextureNode&)graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
					aoTexture.SetTexture(aoProperty.GetConnection(0));
					graph.SetOcclusion(aoTexture.GetR());

					PropertyNode& roughnessProperty = graph.AddProperty("Roughness", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_roughness")));
					SampleTextureNode& roughnessTexture = (SampleTextureNode&)graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
					roughnessTexture.SetTexture(roughnessProperty.GetConnection(0));
					graph.SetRoughness(roughnessTexture.GetR());

					graph.Build();

					ObjectFactory f(l);
					GameObject* sphere = f.Sphere(2, graph.GetMaterial());
					GameObject* sphere2 = f.Sphere(2, Color::White, Transform({ 5, 0, 0 }));
				});

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