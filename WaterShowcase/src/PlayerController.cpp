#include "PlayerController.h"

namespace WaterShowcase
{

	PlayerController::PlayerController(float speed) : Component(),
		Speed(speed)
	{
	
	}

	PlayerController::PlayerController() : PlayerController(5)
	{
	
	}

	void PlayerController::Update()
	{
		Transform& t = gameObject()->transform();
		if (Input::KeyDown(Keycode::A))
		{
			t.Translate(-t.Right() * Speed * Time::DeltaTime());
		}
		else if (Input::KeyDown(Keycode::D))
		{
			t.Translate(t.Right() * Speed * Time::DeltaTime());
		}
		if (Input::KeyDown(Keycode::W))
		{
			t.Translate(t.Forward() * Speed * Time::DeltaTime());
		}
		else if (Input::KeyDown(Keycode::S))
		{
			t.Translate(-t.Forward() * Speed * Time::DeltaTime());
		}
		if (Input::KeyDown(Keycode::Left))
		{
			t.Rotate(PI / 2.0f * Time::DeltaTime(), Vector3f::Up(), Space::World);
		}
		if (Input::KeyDown(Keycode::Right))
		{
			t.Rotate(-PI / 2.0f * Time::DeltaTime(), Vector3f::Up(), Space::World);
		}
		if (Input::KeyDown(Keycode::Up))
		{
			t.Rotate(PI / 2.0f * Time::DeltaTime(), Vector3f::Right(), Space::Local);
		}
		if (Input::KeyDown(Keycode::Down))
		{
			t.Rotate(-PI / 2.0f * Time::DeltaTime(), Vector3f::Right(), Space::Local);
		}
	}

	std::unique_ptr<Component> PlayerController::Clone() const
	{
		std::unique_ptr<PlayerController> p = std::make_unique<PlayerController>();
		p->Speed = Speed;
		return std::move(p);
	}

}