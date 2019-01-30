#include "bltpch.h"
#include "CharacterPrefabId.h"

namespace DND
{

	CharacterPrefabId::CharacterPrefabId(id_t prefabId) : Component(),
		PrefabId(prefabId)
	{
	
	}

	std::unique_ptr<Component> CharacterPrefabId::Clone() const
	{
		return std::make_unique<CharacterPrefabId>(PrefabId);
	}

}