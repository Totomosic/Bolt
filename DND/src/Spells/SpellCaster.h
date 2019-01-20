#pragma once
#include "bltpch.h"

namespace DND
{

	struct GameStateObjects;

	class SpellCaster : public Component
	{
	public:
		SpellCaster();

		void Cast(id_t spellId, const OutputMemoryStream& castData, const GameStateObjects& state);

		std::unique_ptr<Component> Clone() const override;

	};

}