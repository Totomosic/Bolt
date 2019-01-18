#pragma once
#include "bltpch.h"

namespace DND
{

	class SpellCaster : public Component
	{
	public:
		SpellCaster();

		void Cast(id_t spellId, const OutputMemoryStream& castData);

		std::unique_ptr<Component> Clone() const override;

	};

}