#include "bltpch.h"
#include "CharacterAnimator.h"

namespace DND
{

	CharacterAnimator::CharacterAnimator(Direction currentDirection, ResourcePtr<Texture2D> up, ResourcePtr<Texture2D> right, ResourcePtr<Texture2D> down, ResourcePtr<Texture2D> left) : Component(),
		m_CurrentDir(currentDirection), m_DirectionTextures()
	{
		m_DirectionTextures[(int)Direction::Up] = std::move(up);
		m_DirectionTextures[(int)Direction::Right] = std::move(right);
		m_DirectionTextures[(int)Direction::Down] = std::move(down);
		m_DirectionTextures[(int)Direction::Left] = std::move(left);
	}

	Direction CharacterAnimator::CurrentDirection() const
	{
		return m_CurrentDir;
	}

	void CharacterAnimator::SetDirection(Direction direction)
	{
		m_CurrentDir = direction;
		SetCharacterTexture(m_DirectionTextures[(int)m_CurrentDir]);
	}

	void CharacterAnimator::Start()
	{
		SetCharacterTexture(m_DirectionTextures[(int)m_CurrentDir]);
	}

	std::unique_ptr<Component> CharacterAnimator::Clone() const
	{
		std::unique_ptr<CharacterAnimator> c = std::make_unique<CharacterAnimator>(m_CurrentDir, m_DirectionTextures[(int)Direction::Up], m_DirectionTextures[(int)Direction::Right], m_DirectionTextures[(int)Direction::Down], m_DirectionTextures[(int)Direction::Left]);
		return std::move(c);
	}

	void CharacterAnimator::SetCharacterTexture(const ResourcePtr<Texture2D>& texture) const
	{
		Mesh& m = gameObject()->Components().GetComponent<MeshRenderer>().Mesh;
		if (m.Materials[0].Textures.Textures.size() > 0)
		{
			m.Materials[0].Textures.Textures[0] = texture;
		}
		else
		{
			m.Materials[0].Textures.Textures.push_back(texture);
		}
	}

}