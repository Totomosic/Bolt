#include "SpriteSheetAnimator.h"

namespace Bolt
{

	SpriteSheetAnimator::SpriteSheetAnimator(int cols, int rows, float time) : TextureAnimator(),
		m_Rows(rows), m_Columns(cols), m_TotalTime(time), m_CurrentTime(0.0f)
	{
	
	}

	void SpriteSheetAnimator::Update(double elapsedSeconds)
	{
		if (m_CurrentTime >= m_TotalTime)
		{
			m_CurrentTime -= m_TotalTime;
		}
		m_CurrentTime += elapsedSeconds;
	}

	TextureBounds SpriteSheetAnimator::GetBounds() const
	{
		int currentIndex = m_CurrentTime / m_TotalTime * (m_Rows * m_Columns);
		int currentRow = currentIndex / m_Columns;
		int currentCol = currentIndex % m_Rows;
		float imageWidth = 1.0f / m_Columns;
		float imageHeight = 1.0f / m_Rows;
		TextureBounds result;
		result.MinX = currentCol * imageWidth;
		result.MaxX = currentCol * imageWidth + imageWidth;
		result.MaxY = 1.0f - (currentRow * imageHeight);
		result.MinY = 1.0f - (currentRow * imageHeight + imageHeight);
		return result;
	}

	std::unique_ptr<TextureAnimator> SpriteSheetAnimator::Clone() const
	{
		std::unique_ptr<SpriteSheetAnimator> animator = std::make_unique<SpriteSheetAnimator>(m_Rows, m_Columns, m_TotalTime);
		animator->m_CurrentTime = m_CurrentTime;
		return std::move(animator);
	}

}