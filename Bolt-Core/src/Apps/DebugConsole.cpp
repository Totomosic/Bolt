#include "Types.h"
#include "DebugConsole.h"

namespace Bolt
{

	void DebugConsole::Init()
	{
		Scene& scene = SceneManager::Get().CreateScene();
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, -100, 100));
		Layer& layer = scene.CreateLayer(camera);

		layer.UI().Text(">lua", ResourceManager::Get().Fonts().Consolas(36), Color::White, Transform({ 0, camera->ViewHeight(), 0 }), AlignH::Left, AlignV::Top);

		RenderSchedule s(scene);
		s.AddRenderProcess({});
		SceneRenderer::Get().AddRenderSchedule(s);
	}

	void DebugConsole::Update()
	{
	
	}

	void DebugConsole::Render()
	{
		if (m_NeedRedraw)
		{
			Graphics::Get().RenderScene();
			m_NeedRedraw = false;
		}
	}

	void DebugConsole::Exit()
	{
		Application::Exit();
	}

	void DebugConsole::PostMsg(const blt::string& msg)
	{
		
	}

}