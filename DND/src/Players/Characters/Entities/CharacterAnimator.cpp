#include "bltpch.h"
#include "CharacterAnimator.h"

namespace DND
{

	CharacterAnimator::CharacterAnimator(Direction currentDirection, const CharacterTextureSet& textures) : Component(),
		m_Animator(nullptr), m_Textures(textures), m_CurrentDirection(currentDirection)
	{
	
	}

	const CharacterTextureSet& CharacterAnimator::Textures() const
	{
		return m_Textures;
	}

	Direction CharacterAnimator::CurrentDirection() const
	{
		return m_CurrentDirection;
	}

	const ResourcePtr<const Texture2D>& CharacterAnimator::CurrentTexture() const
	{
		return GetTextureByDirection(CurrentDirection());
	}

	const ResourcePtr<const Texture2D>& CharacterAnimator::GetTextureByDirection(Direction direction) const
	{
		switch (direction)
		{
		case Direction::Up:
			return m_Textures.UpTexture;
		case Direction::Down:
			return m_Textures.DownTexture;
		case Direction::Left:
			return m_Textures.LeftTexture;
		case Direction::Right:
			return m_Textures.RightTexture;
		}
		BLT_ASSERT(false, "Invalid Direction");
		return m_Textures.UpTexture;
	}

	void CharacterAnimator::Start()
	{
		m_Animator = &gameObject()->Components().GetComponent<SpriteAnimator>();
		SetCurrentDirection(m_CurrentDirection);
	}

	void CharacterAnimator::SetCurrentDirection(Direction direction)
	{
		m_CurrentDirection = direction;
		m_Animator->SetDefaultTexture(CurrentTexture());
	}

	std::unique_ptr<Component> CharacterAnimator::Clone() const
	{
		return std::make_unique<CharacterAnimator>(m_CurrentDirection, m_Textures);
	}

}