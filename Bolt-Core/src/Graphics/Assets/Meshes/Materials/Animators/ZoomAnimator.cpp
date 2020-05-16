#include "bltpch.h"

#include "ZoomAnimator.h"

namespace Bolt
{

	ZoomAnimator::ZoomAnimator(float xZoom, float yZoom) : TextureAnimator(),
		m_X(xZoom), m_Y(yZoom)
	{
	
	}

	void ZoomAnimator::Update(double elapsedSeconds)
	{
	
	}

	TextureBounds ZoomAnimator::GetBounds() const
	{
		TextureBounds result;
		result.MinX = 0.0f;
		result.MinY = 0.0f;
		result.MaxX = 1.0f / m_X;
		result.MaxY = 1.0f / m_Y;
		return result;
	}

	std::unique_ptr<TextureAnimator> ZoomAnimator::Clone() const
	{
		return std::make_unique<ZoomAnimator>(m_X, m_Y);
	}

}