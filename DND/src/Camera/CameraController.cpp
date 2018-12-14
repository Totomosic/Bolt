#include "CameraController.h"

namespace DND
{

	CameraController::CameraController(float speed) : Component(),
		Speed(speed)
	{
	
	}

	void CameraController::Update()
	{
		Transform& t = gameObject()->transform();
		if (Input::KeyDown(Keycode::W))
		{
			t.Translate(Vector3f::Up() * Speed * Time::RenderingTimeline().DeltaTime());
		}
		if (Input::KeyDown(Keycode::S))
		{
			t.Translate(Vector3f::Up() * -Speed * Time::RenderingTimeline().DeltaTime());
		}
		if (Input::KeyDown(Keycode::D))
		{
			t.Translate(Vector3f::Right() * Speed * Time::RenderingTimeline().DeltaTime());
		}
		if (Input::KeyDown(Keycode::A))
		{
			t.Translate(Vector3f::Right() * -Speed * Time::RenderingTimeline().DeltaTime());
		}
		if (Input::MouseButtonDown(MouseButton::Middle))
		{
			float width = camera()->ViewWidth();
			float height = camera()->ViewHeight();
			float x = Input::RelMousePosition(width, height).x;
			float y = Input::RelMousePosition(width, height).y;
			if (x != 0 || y != 0)
			{
				t.Translate(Vector3f(-x, -y, 0));
			}
		}
	}

	std::unique_ptr<Component> CameraController::Clone() const
	{
		return std::make_unique<CameraController>(Speed);
	}

}