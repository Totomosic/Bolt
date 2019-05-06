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
			int type = Random::NextInt(0, 5);
			if (type == 0)
			{
				float radius = 75;
				float x = Random::NextFloat(ViewWidth / 2, ViewWidth) * (Random::NextInt(0, 1) == 0 ? -1 : 1) + ViewWidth / 2;
				float y = FloorHeight + radius;
				float vx = Random::NextFloat(300, 800) * -abs(x) / x;
				float vy = 0;
				GameObject * meteor = Factory.Ellipse(radius * 2, radius * 2, ResourceManager::Get().Materials().Texture(ResourceManager::Get().GetResource<Texture2D>(GroundTexture)), Transform({ x, y, 0 }));
				meteor->AddTag("Meteor");
				meteor->Components().AddComponent<MeteorController>(radius, 0, Vector2f{ vx, vy }, [](GameObject * object, const Vector3f & position)
					{
						
					});
			}
			else
			{
				float g = 1000;
				float radius = Random::NextFloat(50, 150);
				float x = Random::NextFloat(-ViewWidth, ViewWidth * 2);
				float y = Random::NextFloat(ViewHeight + radius, ViewHeight * 2 + radius);
				float tx = Random::NextFloat(0, ViewWidth);
				float vx = (tx - x) / (sqrt(2 * (y - FloorHeight) / g));
				float vy = 0;
				GameObject * meteor = Factory.Ellipse(radius * 2, radius * 2, ResourceManager::Get().Materials().Texture(ResourceManager::Get().GetResource<Texture2D>(RegularMeteorTexture)), Transform({ x, y, 0 }));
				meteor->AddTag("Meteor");
				meteor->Components().AddComponent<MeteorController>(radius, g, Vector2f{ vx, vy }, [](GameObject * object, const Vector3f & position)
					{
						
					});
			}
			float maxTime = Map<float>(Time::Get().RenderingTimeline().CurrentTime(), 0, 60, 5, 1);
			m_TimeToNextMeteor = Random::NextFloat(maxTime / 10.0f, maxTime);
		}
	}

}