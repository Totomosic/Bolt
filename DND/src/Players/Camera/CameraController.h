#pragma once
#include "bltpch.h"

namespace DND
{

	class CameraController : public Component
	{
	private:
		GameObject* m_Player;

	public:
		CameraController(GameObject* player);

		void Start() override;
		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	private:
		void CenterOnPosition(const Vector2f& position) const;

	};

}