#include "GameManager.h"
#include "MeteorController.h"

namespace Meteors
{

	GameManager::GameManager() : Component(),
		Factory(), Player(nullptr), m_TimeToNextMeteor(3)
	{
	
	}

	void GameManager::Update()
	{
		m_TimeToNextMeteor -= Time::Get().RenderingTimeline().DeltaTime();
		if (m_TimeToNextMeteor <= 0)
		{
			// spawn meteor
			float radius = Random::NextFloat(25, 150);
			float x = Random::NextFloat(0, ViewWidth);
			float y = Random::NextFloat(ViewHeight + radius, ViewHeight * 2 + radius);
			float vx = 0;//Random::NextFloat(10, 100) * -(x - ViewWidth / 2) / (ViewWidth / 2);
			float vy = 0;
			GameObject* meteor = Factory.Ellipse(radius * 2, radius * 2, ResourceManager::Get().Materials().Texture(ResourceManager::Get().Get<Texture2D>(RegularMeteorTexture)), Transform({ x, y, 0 }));
			meteor->Components().AddComponent<MeteorController>(radius, 1000, Vector2f{ vx, vy }, [](GameObject * object, const Vector3f & position)
				{
					
				});
			m_TimeToNextMeteor = Random::NextFloat(0.2f, 2.0f);
		}
	}

}