#include "bltpch.h"
#include "LoadingScene.h"

namespace DND
{

	Scene& CreateLoadingScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::CreateScene("Loading");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);

		id_t loadingIconId = resources.GetResourceId("loadingSymbol");
		UIsurface& background = layer.UI().Rectangle(camera->ViewWidth(), camera->ViewHeight(), Color::White, Transform({ camera->ViewWidth() / 2, camera->ViewHeight() / 2, -50 }));
		UIsurface& loadingIcon = background.Image(300, 300, ResourceManager::Get().GetResource<Texture2D>(loadingIconId), Transform({ 0, 0, 1 }));
		loadingIcon.Object()->Components().AddComponent<TriggerComponent>([](GameObject* object)
		{

		},
			[](GameObject* object) {
			object->transform().Rotate(Quaternion::FromAngleAxis(PI * 4 * Time::RenderingTimeline().DeltaTime(), -Vector3f::Forward()));
		});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
		return scene;
	}

}