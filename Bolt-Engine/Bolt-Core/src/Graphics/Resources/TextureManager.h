#pragma once
#include "Textures/Texture2D.h"
#include "ResourcePtr.h"

namespace Bolt
{

	class ResourceManager;

	class BLT_API TextureManager
	{
	private:
		ResourceManager* m_Manager;

		std::unique_ptr<Texture2D> m_DefaultWhite;
		std::unique_ptr<Texture2D> m_DefaultBlack;
		std::unique_ptr<Texture2D> m_DefaultBlue;

	public:
		TextureManager(ResourceManager* manager);

		ResourcePtr<Texture2D> DefaultWhite() const;
		ResourcePtr<Texture2D> DefaultBlack() const;
		ResourcePtr<Texture2D> DefaultBlue() const;
	};

}