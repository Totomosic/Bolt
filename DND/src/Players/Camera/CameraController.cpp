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
	
	}

	void CameraController::Update()
	{
	
	}

	std::unique_ptr<Component> CameraController::Clone() const
	{
		return std::make_unique<CameraController>(m_Player);
	}

}