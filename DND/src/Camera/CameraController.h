#pragma once
#include "..\BoltInclude.h"

namespace DND
{

	class CameraController : public Component
	{
	public:
		float Speed;

	public:
		CameraController(float speed);

		void Update() override;
		std::unique_ptr<Component> Clone() const override;

	};

}