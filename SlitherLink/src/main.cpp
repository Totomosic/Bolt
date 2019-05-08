#include "Bolt.h"

namespace SlitherLink
{
	
	class App : public Application
	{
	public:
		void Init() override
		{
			Scene& s = SceneManager::Get().CreateScene();
			Camera* camera = s.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, -100, 100));
			Layer& l = s.CreateLayer(camera);

			ObjectFactory f(l);
			f.Ellipse(300, 300, Color::Red, Transform({ 1920 / 2, 1080 / 2, 0 }));

			RenderSchedule sch(s);
			sch.AddRenderProcess({});
			SceneRenderer::Get().AddRenderSchedule(sch);
		}

		void Update() override
		{
		
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