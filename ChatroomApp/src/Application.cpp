#include "BoltEngine.h"

using namespace Bolt;

namespace AStar
{

	class AStarApp : public Application
	{
	public:
		float columns = 20;

	public:
		void Init() override
		{
			Scene* scene = SceneManager::CreateScene();
			Layer* layer = scene->CreateLayer<SceneArray>("Main");
			Camera* camera = scene->CreateCamera(PrimaryWindow->GetFramebuffer().ViewFrustum(-100, 100), ProjectionType::Orthographic);
			layer->SetActiveCamera(camera);

			ObjectFactory factory(layer);
			
			int rows = ((int)columns) / PrimaryWindow->GetFramebuffer().Aspect();
			RedrawBoard(factory, columns, rows);

			id_t renderer = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));//Graphics::AddRenderer(std::make_unique<SpriteRenderer2D>());
			Graphics::Schedule().RenderPasses.push_back({ Graphics::DefaultFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(renderer) });
		}

		void Tick() override
		{
			BLT_INFO(Time::FramesPerSecond());
		}

		void Update() override
		{
			int oldCols = (int)columns;
			columns -= Input::RelMouseScroll().y;
			if ((int)columns != oldCols)
			{
				ObjectFactory factory(&SceneManager::CurrentScene().GetLayer("Main"));
				int rows = (columns) / PrimaryWindow->GetFramebuffer().Aspect();
				RedrawBoard(factory, columns, rows);
			}
		}

		void Render() override
		{
			Graphics::RenderScene();
		}

		void RedrawBoard(const ObjectFactory& factory, int columns, int rows)
		{
			factory.CurrentLayer()->GameObjects().RemoveAllWithTag("Tile");
			float border = 2;
			float width = (Width() - (columns + 1) * border) / (columns);
			float height = (Height() - (rows + 1) * border) / (rows);
			for (int i = 0; i < columns; i++)
			{
				for (int j = 0; j < rows; j++)
				{
					float x = border + i * (width + border) + width / 2.0f;
					float y = border + j * (height + border) + height / 2.0f;
					Material mat;
					mat.BaseColor = (Random::NextInt(0, 10) == 2) ? Color::Random() : Color::White;
					GameObject* tile = factory.Rectangle(width, height, mat, Transform({ x, y, 0 }));
					tile->AddTag("Tile");
				}
			}
		}

	};

}

int main()
{
	Engine e;
	e.SetWindow(std::make_unique<Window>(1280, 720, "A* Visualisation"));
	e.SetApplication(std::make_unique<AStar::AStarApp>());
	e.Run();
}