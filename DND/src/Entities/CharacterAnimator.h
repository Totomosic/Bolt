#pragma once
#include "SpriteAnimator.h"

namespace DND
{

	enum class Direction
	{
		Up,
		Right,
		Down,
		Left,
		MAX_DIRECTIONS
	};

	class CharacterAnimator : public Component
	{
	private:
		ResourcePtr<Texture2D> m_DirectionTextures[(int)Direction::MAX_DIRECTIONS];
		Direction m_CurrentDir;

	public:
		CharacterAnimator(ResourcePtr<Texture2D> up, ResourcePtr<Texture2D> right, ResourcePtr<Texture2D> down, ResourcePtr<Texture2D> left);

		Direction CurrentDirection() const;
		void SetDirection(Direction direction);

		std::unique_ptr<Component> Clone() const override;

	};

}