#include "bltpch.h"
#include "CameraController.h"

namespace DND
{

	CameraController::CameraController(GameObject* player) : Component(),
		m_Player(player)
	{
		
	}

	void CameraController::Start()
	{
		Update();
	}

	void CameraController::Update()
	{
		Vector3f playerPosition = m_Player->transform().Position();
		CenterOnPosition(playerPosition.xy());
	}

	std::unique_ptr<Component> CameraController::Clone() const
	{
		return std::make_unique<CameraController>(m_Player);
	}

	void CameraController::CenterOnPosition(const Vector2f& position) const
	{
		Camera& c = *camera();
		gameObject()->transform().SetLocalXY(position.x - c.ViewWidth() / 2.0f, position.y - c.ViewHeight() / 2.0f);
	}

}