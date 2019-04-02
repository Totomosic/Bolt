#pragma once
#include "Textures/Fonts/__Fonts__.h"
#include "ResourcePtr.h"

namespace Bolt
{

	class BLT_API FontManager
	{
	private:
		std::unique_ptr<FontFactory> m_ArialFactory;
		mutable std::unordered_map<float, ResourcePtr<const Font>> m_ArialFonts;

	public:
		FontManager();

		ResourcePtr<const Font> Arial(float fontSize) const;

		friend class ResourceManager;

	private:
		void Initialize();
		void CreateArialFont();

	};

}