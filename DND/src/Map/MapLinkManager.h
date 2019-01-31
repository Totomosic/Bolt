#pragma once
#include "bltpch.h"
#include "TilemapLayer.h"

namespace DND
{

	class MapLink
	{
	public:
		using ValidationFunc = std::function<bool(GameObject*)>;

	private:
		id_t m_FromMapId;
		id_t m_ToMapId;
		Tile m_FromTile;
		Tile m_ToTile;
		ValidationFunc m_Validator;

	public:
		MapLink();
		MapLink(id_t fromMap, id_t toMap, Tile fromTile, Tile toTile, ValidationFunc validator = [](GameObject* object)
		{
			return true;
		});

		id_t FromMapId() const;
		id_t ToMapId() const;
		const Tile& FromTile() const;
		const Tile& ToTile() const;

		void TestObject(GameObject* object) const;

	};

	class MapLinkManager
	{
	private:
		std::vector<MapLink> m_Links;

	public:
		MapLinkManager();

	};

}