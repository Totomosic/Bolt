#pragma once
#include "bltpch.h"

namespace DND
{

	class CharacterPrefabId : public Component
	{
	public:
		id_t PrefabId;

	public:
		CharacterPrefabId(id_t prefabId);

		std::unique_ptr<Component> Clone() const override;

	};

}