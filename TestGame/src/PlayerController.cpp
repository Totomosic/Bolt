#include "PlayerController.h"
#include "RigidBody.h"

namespace MeteorGame
{

	PlayerController::PlayerController() : Component(),
		Speed(400)
	{
	
	}

	void PlayerController::Update()
	{
		RigidBody& rb = gameObject()->Components().GetComponent<RigidBody>();
		if (Input::KeyDown(Keycode::A))
		{
			rb.Velocity.x = -Speed;
		}
		else if (Input::KeyDown(Keycode::D))
		{
			rb.Velocity.x = Speed;
		}
		else
		{
			rb.Velocity.x = 0.0f;
		}

		if (Input::KeyDown(Keycode::W) && gameObject()->transform().Position().y == 100 + 75)
		{
			rb.Velocity.y = 500;
			rb.Acceleration.y = -1000;
		}
		if (gameObject()->transform().Position().y < 100 + 75)
		{
			rb.Velocity.y = 0;
			rb.Acceleration.y = 0;
			gameObject()->transform().SetLocalPosition(gameObject()->transform().Position().x, 100 + 75, gameObject()->transform().Position().z);
		}
	}

	std::unique_ptr<Component> PlayerController::Clone() const
	{
		std::unique_ptr<PlayerController> p = std::make_unique<PlayerController>();
		p->Speed = Speed;
		return std::move(p);
	}

}