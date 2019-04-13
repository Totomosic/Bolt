#include "Types.h"
#include "FontManager.h"
#include "ResourceManager.h"

namespace Bolt
{

	FontManager::FontManager()
		: m_ArialFactory("C:/Windows/Fonts/arial.ttf"), m_ArialFonts(),
		m_CalibriFactory("C:/Windows/Fonts/calibri.ttf"), m_CalibriFonts(),
		m_TimesFactory("C:/Windows/Fonts/times.ttf"), m_TimesFonts(),
		m_VerdanaFactory("C:/Windows/Fonts/verdana.ttf"), m_VerdanaFonts()
	{
		
	}

	ResourcePtr<const Font> FontManager::Arial(float fontSize) const
	{
		if (m_ArialFonts.find(fontSize) != m_ArialFonts.end())
		{
			return m_ArialFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_ArialFactory.BuildFont(fontSize);
		ResourcePtr<const Font> font = ResourceManager::Get().Get<Font>(ResourceManager::Get().Register(std::move(f)));
		m_ArialFonts[fontSize] = font;
		return font;
	}

	ResourcePtr<const Font> FontManager::Calibri(float fontSize) const
	{
		if (m_CalibriFonts.find(fontSize) != m_CalibriFonts.end())
		{
			return m_CalibriFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_CalibriFactory.BuildFont(fontSize);
		ResourcePtr<const Font> font = ResourceManager::Get().Get<Font>(ResourceManager::Get().Register(std::move(f)));
		m_CalibriFonts[fontSize] = font;
		return font;
	}

	ResourcePtr<const Font> FontManager::TimesNewRoman(float fontSize) const
	{
		if (m_TimesFonts.find(fontSize) != m_TimesFonts.end())
		{
			return m_TimesFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_TimesFactory.BuildFont(fontSize);
		ResourcePtr<const Font> font = ResourceManager::Get().Get<Font>(ResourceManager::Get().Register(std::move(f)));
		m_TimesFonts[fontSize] = font;
		return font;
	}

	ResourcePtr<const Font> FontManager::Verdana(float fontSize) const
	{
		if (m_VerdanaFonts.find(fontSize) != m_VerdanaFonts.end())
		{
			return m_VerdanaFonts.at(fontSize);
		}
		std::unique_ptr<Font> f = m_VerdanaFactory.BuildFont(fontSize);
		ResourcePtr<const Font> font = ResourceManager::Get().Get<Font>(ResourceManager::Get().Register(std::move(f)));
		m_VerdanaFonts[fontSize] = font;
		return font;
	}

}