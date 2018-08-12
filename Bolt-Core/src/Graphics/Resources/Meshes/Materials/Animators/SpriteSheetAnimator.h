#pragma once
#include "..\TextureAnimator.h"

namespace Bolt
{

	class BLT_API SpriteSheetAnimator : public TextureAnimator
	{
	private:
		int m_Rows;
		int m_Columns;
		float m_TotalTime;
		float m_CurrentTime;

	public:
		SpriteSheetAnimator(int cols, int rows, float time);

		void Update(double elapsedSeconds) override;
		TextureBounds GetBounds() const override;
		std::unique_ptr<TextureAnimator> Clone() const override;

	};

}