#pragma once
#include "Textures/Fonts/Font.h"
#include "Textures/Fonts/FontFactory.h"
#include "AssetHandle.h"

namespace Bolt
{

	class AssetManager;

	class BLT_API FontManager
	{
	private:
		AssetManager* m_Manager;

		FontFactory m_ArialFactory;
		mutable std::unordered_map<float, AssetHandle<Font>> m_ArialFonts;
		FontFactory m_CalibriFactory;
		mutable std::unordered_map<float, AssetHandle<Font>> m_CalibriFonts;
		FontFactory m_ConsolasFactory;
		mutable std::unordered_map<float, AssetHandle<Font>> m_ConsolasFonts;
		FontFactory m_LucidaFactory;
		mutable std::unordered_map<float, AssetHandle<Font>> m_LucidaFonts;
		FontFactory m_TimesFactory;
		mutable std::unordered_map<float, AssetHandle<Font>> m_TimesFonts;
		FontFactory m_VerdanaFactory;
		mutable std::unordered_map<float, AssetHandle<Font>> m_VerdanaFonts;
		mutable AssetHandle<Font> m_DefaultFont;

	public:
		FontManager(AssetManager* manager);

		AssetHandle<Font> Default() const;
		void SetDefault(const AssetHandle<Font>& defaultFont) const;
		AssetHandle<Font> Arial(float fontSize) const;
		AssetHandle<Font> Calibri(float fontSize) const;
		AssetHandle<Font> Consolas(float fontSize) const;
		AssetHandle<Font> Lucida(float fontSize) const;
		AssetHandle<Font> TimesNewRoman(float fontSize) const;
		AssetHandle<Font> Verdana(float fontSize) const;

	};

}