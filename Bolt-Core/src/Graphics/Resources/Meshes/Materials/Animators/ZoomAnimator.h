#pragma once
#include "..\TextureAnimator.h"

namespace Bolt
{

	class BLT_API ZoomAnimator : public TextureAnimator
	{
	private:
		float m_X;
		float m_Y;

	public:
		ZoomAnimator(float xZoom, float yZoom);

		void Update(double elapsedSeconds) override;
		TextureBounds GetBounds() const override;
		std::unique_ptr<TextureAnimator> Clone() const override;

	};

}