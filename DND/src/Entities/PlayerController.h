#pragma once
#include "TileMotion.h"
#include "CharacterAnimator.h"

namespace DND
{

	class PlayerController : public Component
	{
	private:
		Keycode m_Left;
		Keycode m_Up;
		Keycode m_Right;
		Keycode m_Down;

	public:
		PlayerController();

		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}