#pragma once
#include "Textures/Fonts/Font.h"
#include "Textures/Fonts/FontFactory.h"
#include "ResourcePtr.h"

namespace Bolt
{

	class BLT_API FontManager
	{
	private:
		FontFactory m_ArialFactory;
		mutable std::unordered_map<float, ResourcePtr<Font>> m_ArialFonts;
		FontFactory m_CalibriFactory;
		mutable std::unordered_map<float, ResourcePtr<Font>> m_CalibriFonts;
		FontFactory m_ConsolasFactory;
		mutable std::unordered_map<float, ResourcePtr<Font>> m_ConsolasFonts;
		FontFactory m_LucidaFactory;
		mutable std::unordered_map<float, ResourcePtr<Font>> m_LucidaFonts;
		FontFactory m_TimesFactory;
		mutable std::unordered_map<float, ResourcePtr<Font>> m_TimesFonts;
		FontFactory m_VerdanaFactory;
		mutable std::unordered_map<float, ResourcePtr<Font>> m_VerdanaFonts;

	public:
		FontManager();

		ResourcePtr<Font> Arial(float fontSize) const;
		ResourcePtr<Font> Calibri(float fontSize) const;
		ResourcePtr<Font> Consolas(float fontSize) const;
		ResourcePtr<Font> Lucida(float fontSize) const;
		ResourcePtr<Font> TimesNewRoman(float fontSize) const;
		ResourcePtr<Font> Verdana(float fontSize) const;

	};

}