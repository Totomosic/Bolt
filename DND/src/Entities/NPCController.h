#pragma once
#include "TileMotion.h"
#include "CharacterAnimator.h"

namespace DND
{

	class NPCController : public Component
	{
	public:
		NPCController();

		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}