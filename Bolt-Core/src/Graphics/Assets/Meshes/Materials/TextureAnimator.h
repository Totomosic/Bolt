#pragma once
#include "Graphics/Assets/Textures/Texture2D.h"

namespace Bolt
{

	class BLT_API TextureAnimator
	{
	protected:
		bool m_IsPaused = false;

	public:
		inline bool IsPaused() const { return m_IsPaused; }
		inline void Pause() { m_IsPaused = true; }
		inline void Resume() { m_IsPaused = false; }

		virtual void Update(double elapsedTime) = 0;
		virtual TextureBounds GetBounds() const = 0;
		virtual std::unique_ptr<TextureAnimator> Clone() const = 0;

	};

}