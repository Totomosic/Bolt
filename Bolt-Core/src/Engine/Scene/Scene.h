#pragma once
#include "Layer.h"

namespace Bolt
{

	class BLT_API Scene
	{
	private:
		std::vector<std::unique_ptr<Layer>> m_Layers;
		
	public:
		Scene();

		Layer& AddLayer();
		void RemoveLayer(Layer& layer);

		void Load();
		void Unload();
		void Update(TimeDelta delta);
		void Render(TimeDelta delta);
	};

}