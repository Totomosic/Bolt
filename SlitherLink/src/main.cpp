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
			camera->transform().Translate(camera->transform().Forward() * -5);

			ResourceManager::Get().LoadPack("res/resources.pack", [&l](const ResourcePack& resources)
				{
					PBRMaterialGraph graph;
					PropertyNode& albedoProperty = graph.AddProperty("Albedo", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_basecolor")));
					SampleTextureNode& albedoTexture = graph.AddNode(std::make_unique<SampleTextureNode>());
					albedoTexture.SetTexture(albedoProperty.GetValue());
					graph.SetAlbedo(albedoTexture.GetRGB());

					PropertyNode& metallicProperty = graph.AddProperty("Metallic", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_metallic")));
					SampleTextureNode& metallicTexture = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
					metallicTexture.SetTexture(metallicProperty.GetValue());
					graph.SetMetallic(metallicTexture.GetR());

					PropertyNode& aoProperty = graph.AddProperty("AO", ResourceManager::Get().Textures().DefaultWhite());
					SampleTextureNode& aoTexture = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
					aoTexture.SetTexture(aoProperty.GetValue());
					graph.SetOcclusion(aoTexture.GetR());

					PropertyNode& roughnessProperty = graph.AddProperty("Roughness", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_roughness")));
					SampleTextureNode& roughnessTexture = graph.AddNode(std::make_unique<SampleTextureNode>(SampleMode::Normal));
					roughnessTexture.SetTexture(roughnessProperty.GetValue());
					graph.SetRoughness(roughnessTexture.GetR());

					graph.Build();

					ObjectFactory f(l);
					GameObject* sphere = f.Sphere(2, graph.GetMaterial());

					LitMaterialGraph litGraph;
					PropertyNode& colorProperty = litGraph.AddProperty("Texture", ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("rustediron2_basecolor")));
					SampleTextureNode& litSampler = litGraph.AddNode<SampleTextureNode>();
					litSampler.SetTexture(colorProperty.GetValue());
					litGraph.SetColor(litSampler.GetRGB());
					litGraph.Build();

					GameObject* sphere2 = f.Sphere(2, litGraph.GetMaterial(), Transform({ 5, 0, 0 }));
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