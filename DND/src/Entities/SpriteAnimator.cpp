#include "bltpch.h"
#include "SpriteAnimator.h"

namespace DND
{

	SpriteAnimator::SpriteAnimator(ResourcePtr<Texture2D> defaultTexture) : Component(),
		m_CurrentAnimation{ { }, -1.0f }, m_DefaultTexture(std::move(defaultTexture)), m_Material(nullptr)
	{
		
	}

	const SpriteAnimator::AnimationInfo& SpriteAnimator::CurrentAnimation() const
	{
		return m_CurrentAnimation;
	}

	bool SpriteAnimator::IsPlayingAnimation() const
	{
		return m_CurrentAnimation.RemainingTime > 0.0f;
	}

	const ResourcePtr<Texture2D>& SpriteAnimator::DefaultTexture() const
	{
		return m_DefaultTexture;
	}

	void SpriteAnimator::Start()
	{
		m_Material = &gameObject()->Components().GetComponent<MeshRenderer>().Mesh.Materials[0];
		m_Material->Textures.Textures.resize(1);
		m_Material->Textures.Textures[0] = m_DefaultTexture;
		m_Material->Textures.Animators.clear();
	}

	void SpriteAnimator::Update()
	{
		if (IsPlayingAnimation())
		{
			m_CurrentAnimation.RemainingTime -= Time::RenderingTimeline().DeltaTime();
			if (m_CurrentAnimation.RemainingTime <= 0.0f)
			{
				m_Material->Textures.Textures[0] = m_DefaultTexture;
				m_Material->Textures.Animators.clear();
				m_CurrentAnimation.RemainingTime = -1.0f;
			}
		}
	}

	void SpriteAnimator::PlayAnimation(SpriteAnimation animation, float time)
	{
		PlayAnimationUntilStopped(std::move(animation), time);
		m_CurrentAnimation.RemainingTime = time;
	}

	void SpriteAnimator::PlayAnimationUntilStopped(SpriteAnimation animation, float repeatTime)
	{
		m_CurrentAnimation = { std::move(animation), -1.0f };
		m_Material->Textures.Animators.clear();
		m_Material->Textures.Textures[0] = m_CurrentAnimation.Animation.Animation;
		m_Material->Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(m_CurrentAnimation.Animation.Columns, m_CurrentAnimation.Animation.Rows, repeatTime);
	}

	void SpriteAnimator::StopAnimation()
	{
		m_CurrentAnimation.RemainingTime = -1.0f;
		m_Material->Textures.Animators.clear();
		m_Material->Textures.Textures[0] = m_DefaultTexture;
	}

	void SpriteAnimator::SetDefaultTexture(ResourcePtr<Texture2D> defaultTexture)
	{
		m_DefaultTexture = std::move(defaultTexture);
		if (!IsPlayingAnimation())
		{
			m_Material->Textures.Textures[0] = m_DefaultTexture;
		}
	}

	std::unique_ptr<Component> SpriteAnimator::Clone() const
	{
		std::unique_ptr<SpriteAnimator> animator = std::make_unique<SpriteAnimator>(m_DefaultTexture);
		animator->m_CurrentAnimation = m_CurrentAnimation;
		return std::move(animator);
	}

}