#pragma once
#include "Map/TilemapManager.h"

namespace DND
{

	class DndInstance : public Component
	{
	private:
		TilemapManager m_Tilemap;

	public:
		DndInstance(Layer& layer, const ResourcePtr<Texture2D>& tileset);

		void Start() override;
		void End() override;

		std::unique_ptr<Component> Clone() const override;

	};

}