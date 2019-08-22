#include "mcpch.h"
#include "World/Chunks/Block.h"
#include "World/Chunks/BlockDatabase.h"
#include "World/Chunks/ChunkManager.h"

namespace Minecraft
{

	float BLOCK_SIZE = 1.0f;
	
	class App : public Application
	{
	private:
		Camera* camera;
		std::unique_ptr<ChunkManager> manager;

	public:
		void Init() override
		{
			GetWindow().SetClearColor(Color::CornflowerBlue);
			Scene& s = SceneManager::Get().CreateScene();
			camera = s.CreateCamera(Projection::Perspective(PI / 3, GetWindow().Aspect(), 0.1f, 5000.0f));
			Layer& l = s.CreateLayer(camera);

			Camera* uiCamera = s.CreateCamera(Projection::Orthographic(0, Width(), 0, Height(), -100, 100));
			Layer& uiLayer = s.CreateLayer(uiCamera);

			UISurface& surface = uiLayer.UI().Root().CreateSurface(300, 300, Color::Red, Transform({ 200, 200, 0 }));
			UITextInput& input = surface.CreateTextInput(300, 100, Color::Black, Transform({ 0, 0, 1 }));

			camera->transform().Translate(32, 200, 32);
			camera->transform().Rotate(-PI / 2, Vector3f::Right());

			ResourceManager::Get().LoadPack("res/resources.pack", [&l, this](const ResourcePack& pack)
				{
					ResourceExtractor resources(pack);
					TextureAtlas atlas(resources.GetResourcePtr<Texture2D>("DefaultBlockFaces"), 16, 16);
					BlockDatabase::Initialize(atlas);
					ObjectFactory f(l);
					
					manager = std::make_unique<ChunkManager>(f, atlas.GetTexture(), 4, 4);
					ChunkRegion& chunk = manager->GetChunkRegion();
					TaskManager::Run([&chunk]()
						{
							SimplexNoise noise;
							Vector2f seed(10, 10);
							for (int x = 0; x < chunk.GetWidthInBlocks(); x++)
							{
								for (int z = 0; z < chunk.GetHeightInBlocks(); z++)
								{
									float height = noise.Generate(8, x / 150.0f + seed.x, z / 150.0f + seed.y);
									int blockHeight = 65 + height * 10;
									for (int y = 0; y < chunk.GetDepthInBlocks(); y++)
									{
										if (y < blockHeight - 5)
										{
											chunk.SetBlockId(x, y, z, BlockId::Stone);
										}
										else if (y < blockHeight)
										{
											chunk.SetBlockId(x, y, z, BlockId::Dirt);
										}
										else if (y == blockHeight)
										{
											chunk.SetBlockId(x, y, z, BlockId::Grass);
										}
										else
										{
											chunk.SetBlockId(x, y, z, BlockId::Air);
										}
									}
								}
							}
						}).ContinueWithOnMainThread([manager{ manager.get() }]()
							{
								manager->BuildAllChunks();
							});
				});

			RenderProcess p;
			LightSource light;
			light.Position = Vector3f(0, 10000, 0);
			light.AmbientIntensity = 0.6f;
			light.Intensity = 2.0f;
			p.Options.GlobalContext.Lights.push_back(light);
			RenderSchedule sch(s);
			sch.AddRenderProcess(p);
			SceneRenderer::Get().AddRenderSchedule(sch);
		}

		void Update() override
		{
			Transform& t = camera->transform();
			static float speed = 15;
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
	eng.SetApplication<Minecraft::App>();
	eng.Run();
}