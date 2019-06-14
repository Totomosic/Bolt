#include "mcpch.h"
#include "World/Chunks/Block.h"
#include "World/Chunks/BlockDatabase.h"
#include "World/Chunks/ChunkData.h"

namespace Minecraft
{

	float BLOCK_SIZE = 1.0f;
	
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

			camera->transform().Translate(0, 0, 10);

			ResourceManager::Get().LoadPack("res/resources.pack", [&l](const ResourcePack& pack)
				{
					ResourceExtractor resources(pack);
					TextureAtlas atlas(resources.GetResourcePtr<Texture2D>("DefaultBlockFaces"), 16, 16);
					BlockDatabase::Initialize(atlas);
					ObjectFactory f(l);
					
					ChunkData chunk(16, 16, 16);
					chunk.SetBlockId(15, 8, 8, BlockId::Air);
					auto faces = chunk.GetFaces();
					ModelData data;
					data.Vertices = std::make_unique<VertexArray>();
					data.Indices = std::make_unique<IndexArray>();
					data.Indices->AddIndexBuffer(std::make_unique<IndexBuffer>(faces.size() * 6));
					BufferLayout layout = BufferLayout::Default();
					VertexBuffer& buffer = data.Vertices->CreateVertexBuffer(faces.size() * 4 * layout.Size(), layout);
					Vector4<byte> color = Color::White.ToBytes();
					if (faces.size() > 0)
					{
						VertexMapping vMapping = data.Vertices->Map();
						IndexMapping iMapping = data.Indices->Map();
						VertexIterator it = vMapping.Begin();
						IndexIterator indices = iMapping.Begin();
						for (int i = 0; i < faces.size(); i++)
						{
							BlockFace& face = faces.at(i);
							it[0] = face.TopLeft;
							it[1] = face.Normal;
							it[2] = Vector2f(face.Texture.Min.x, face.Texture.Max.y);
							it[3] = color;
							it++;
							it[0] = face.BottomLeft;
							it[1] = face.Normal;
							it[2] = face.Texture.Min;
							it[3] = color;
							it++;
							it[0] = face.BottomRight;
							it[1] = face.Normal;
							it[2] = Vector2f(face.Texture.Max.x, face.Texture.Min.y);
							it[3] = color;
							it++;
							it[0] = face.TopRight;
							it[1] = face.Normal;
							it[2] = face.Texture.Max;
							it[3] = color;
							it++;
							*indices = (uint32_t)i * 4 + 0;
							indices++;
							*indices = (uint32_t)i * 4 + 1;
							indices++;
							*indices = (uint32_t)i * 4 + 2;
							indices++;
							*indices = (uint32_t)i * 4 + 0;
							indices++;
							*indices = (uint32_t)i * 4 + 2;
							indices++;
							*indices = (uint32_t)i * 4 + 3;
							indices++;
						}
					}
					ResourcePtr<Model> model = ResourceManager::Get().Register(std::make_unique<Model>(std::move(data)));
					Mesh m;
					m.Models.push_back({ model });
					auto material = ResourceManager::Get().Materials().TextureLighting();
					material->GetLinkContext().Link("Texture", atlas.GetTexture());
					m.Materials.push_back(std::move(material));
					f.Instantiate(m);
				});

			RenderProcess p;
			LightSource light;
			light.Position = Vector3f(0, 10000, 0);
			light.AmbientIntensity = 0.5f;
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
	eng.SetApplication<Minecraft::App>();
	eng.Run();
}