#pragma once
#include "bltpch.h"
#include "GamePlayPacketSerialization.h"

namespace DND
{

	class NetworkController : public Component
	{
	public:
		NetworkController();

		void MoveToTile(const Tile& tile) const;
		void CastSpell(id_t spellId, const OutputMemoryStream& castData) const;
		void UpdateStats(const CharacterStats& stats) const;

		std::unique_ptr<Component> Clone() const override;

	};

}