#pragma once
#include "Bolt.h"
#include "Constants.h"

namespace Meteors
{

	class GameManager : public Component
	{
	public:
		ObjectFactory Factory;
		GameObject* Player;

	private:
		float m_TimeToNextMeteor;

	public:
		GameManager();

		void Update() override;

		BLT_CANNOT_CLONE_COMPONENT(GameManager);
	};

}