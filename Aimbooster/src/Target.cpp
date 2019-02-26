#include "Target.h"

namespace Aimbooster
{

	Target::Target(float lifetime, float maxSize, ObjectFactory* factory) : Component(),
		Lifetime(lifetime), StartingSize(1.0f), MaxSize(maxSize), CurrentTime(0.0f), CurrentSize(StartingSize), Factory(factory), Paused(false)
	{
	
	}

	void Target::Start()
	{
		StartingSize = gameObject()->transform().Scale().x;
		CurrentSize = StartingSize;
	}

	void Target::Update()
	{
		if (!Paused)
		{
			CurrentTime += Time::RenderingTimeline().DeltaTime();
			float size = StartingSize + (MaxSize - StartingSize) * sin(CurrentTime / Lifetime * PI);
			gameObject()->transform().SetLocalScale(size, size, 1);
			CurrentSize = size;
			if (CurrentTime >= Lifetime)
			{
				Destroy(gameObject());
				std::unique_ptr<TargetFailedEvent> args = std::make_unique<TargetFailedEvent>();
				args->Position = gameObject()->transform().Position();
				EventManager::Post(TARGET_FAILED_EVENT, std::move(args));
			}
			else
			{
				if (Input::Get().MouseButtonPressed(MouseButton::Left))
				{
					float width = camera()->ViewWidth();
					float height = camera()->ViewHeight();
					if (Vector2f::Distance(Input::Get().MousePosition(width, height).xy(), gameObject()->transform().Position().xy()) <= CurrentSize)
					{
						Destroy(gameObject());
						GameObject* hitLocation = Factory->Ellipse(5, 5, Color::White, Transform({ Input::Get().MousePosition(width, height).x, Input::Get().MousePosition(width, height).y, gameObject()->transform().Position().z + 1 }));
						Destroy(hitLocation, 1.0f);
						EventManager::Post(TARGET_HIT_EVENT);
					}
				}
			}
		}
	}

	std::unique_ptr<Component> Target::Clone() const
	{
		return std::make_unique<Target>(Lifetime, MaxSize, Factory);
	}

}