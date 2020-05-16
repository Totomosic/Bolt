#include "bltpch.h"
#include "FontManager.h"
#include "AssetManager.h"

namespace Bolt
{

	FontManager::FontManager(AssetManager* manager)
		: m_Manager(manager),
		m_ArialFactory("C:/Windows/Fonts/arial.ttf"), m_ArialFonts(),
		m_CalibriFactory("C:/Windows/Fonts/calibri.ttf"), m_CalibriFonts(),
		m_ConsolasFactory("C:/Windows/Fonts/consola.ttf"), m_ConsolasFonts(),
		m_LucidaFactory("C:/Windows/Fonts/ltype.ttf"), m_LucidaFonts(),
		m_TimesFactory("C:/Windows/Fonts/times.ttf"), m_TimesFonts(),
		m_VerdanaFactory("C:/Windows/Fonts/verdana.ttf"), m_VerdanaFonts()
	{
		m_DefaultFont = Arial(48);
	}

	AssetHandle<Font> FontManager::Default() const
	{
		return m_DefaultFont;
	}

	void FontManager::SetDefault(const AssetHandle<Font>& defaultFont) const
	{
		m_DefaultFont = defaultFont;
	}

	AssetHandle<Font> FontManager::Arial(float fontSize) const
	{
		if (m_ArialFonts.find(fontSize) != m_ArialFonts.end())
		{
			return m_ArialFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_ArialFactory.BuildFont(fontSize);
		AssetHandle<Font> font = m_Manager->Register(std::move(f));
		m_ArialFonts[fontSize] = font;
		return font;
	}

	AssetHandle<Font> FontManager::Calibri(float fontSize) const
	{
		if (m_CalibriFonts.find(fontSize) != m_CalibriFonts.end())
		{
			return m_CalibriFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_CalibriFactory.BuildFont(fontSize);
		AssetHandle<Font> font = m_Manager->Register(std::move(f));
		m_CalibriFonts[fontSize] = font;
		return font;
	}

	AssetHandle<Font> FontManager::Consolas(float fontSize) const
	{
		if (m_ConsolasFonts.find(fontSize) != m_ConsolasFonts.end())
		{
			return m_ConsolasFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_ConsolasFactory.BuildFont(fontSize);
		AssetHandle<Font> font = m_Manager->Register(std::move(f));
		m_ConsolasFonts[fontSize] = font;
		return font;
	}

	AssetHandle<Font> FontManager::Lucida(float fontSize) const
	{
		if (m_LucidaFonts.find(fontSize) != m_LucidaFonts.end())
		{
			return m_LucidaFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_LucidaFactory.BuildFont(fontSize);
		AssetHandle<Font> font = m_Manager->Register(std::move(f));
		m_LucidaFonts[fontSize] = font;
		return font;
	}

	AssetHandle<Font> FontManager::TimesNewRoman(float fontSize) const
	{
		if (m_TimesFonts.find(fontSize) != m_TimesFonts.end())
		{
			return m_TimesFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_TimesFactory.BuildFont(fontSize);
		AssetHandle<Font> font = m_Manager->Register(std::move(f));
		m_TimesFonts[fontSize] = font;
		return font;
	}

	AssetHandle<Font> FontManager::Verdana(float fontSize) const
	{
		if (m_VerdanaFonts.find(fontSize) != m_VerdanaFonts.end())
		{
			return m_VerdanaFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_VerdanaFactory.BuildFont(fontSize);
		AssetHandle<Font> font = m_Manager->Register(std::move(f));
		m_VerdanaFonts[fontSize] = font;
		return font;
	}

}