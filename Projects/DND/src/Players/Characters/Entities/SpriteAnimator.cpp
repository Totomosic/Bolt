#include "bltpch.h"
#include "SpriteAnimator.h"

namespace DND
{

	SpriteAnimator::SpriteAnimator() : SpriteAnimator(nullptr)
	{
		
	}

	SpriteAnimator::SpriteAnimator(const ResourcePtr<const Texture2D>& defaultTexture) : Component(),
		m_Mesh(nullptr), m_DefaultTexture(defaultTexture), m_CurrentAnimation({ {}, 0.0f })
	{

	}

	const ResourcePtr<const Texture2D>& SpriteAnimator::DefaultTexture() const
	{
		return m_DefaultTexture;
	}

	const AnimationInfo& SpriteAnimator::CurrentAnimation() const
	{
		return m_CurrentAnimation.Animation;
	}

	bool SpriteAnimator::IsPlayingAnimation() const
	{
		return m_CurrentAnimation.RemainingTime != 0.0f;
	}

	float SpriteAnimator::RemainingAnimationTime() const
	{
		return m_CurrentAnimation.RemainingTime;
	}

	bool SpriteAnimator::IsPlayingUntilStopped() const
	{
		return m_CurrentAnimation.RemainingTime < 0.0f;
	}

	void SpriteAnimator::Start()
	{
		m_Mesh = &gameObject()->Components().GetComponent<MeshRenderer>().Mesh;
		UseDefaultTexture();
	}

	void SpriteAnimator::Update()
	{
		if (IsPlayingAnimation())
		{
			if (m_CurrentAnimation.RemainingTime > 0.0f)
			{
				m_CurrentAnimation.RemainingTime -= Time::RenderingTimeline().DeltaTime();
				if (m_CurrentAnimation.RemainingTime <= 0.0f)
				{
					UseDefaultTexture();
					m_CurrentAnimation.RemainingTime = 0.0f;
				}
			}
		}
	}

	void SpriteAnimator::SetDefaultTexture(const ResourcePtr<const Texture2D>& texture)
	{
		m_DefaultTexture = texture;
		if (!IsPlayingAnimation())
		{
			UseDefaultTexture();
		}
	}

	void SpriteAnimator::PlayAnimation(const AnimationInfo& animation, float repeatTime, float animationDuration)
	{
		m_CurrentAnimation = { animation, repeatTime, animationDuration };
		UseAnimation();
	}

	void SpriteAnimator::PlayAnimation(const AnimationInfo& animation, float repeatTime)
	{
		PlayAnimation(animation, repeatTime, repeatTime);
	}

	void SpriteAnimator::PlayAnimationUntilStopped(const AnimationInfo& animation, float repeatTime)
	{
		PlayAnimation(animation, repeatTime, -1.0f);
	}

	std::unique_ptr<Component> SpriteAnimator::Clone() const
	{
		return std::make_unique<SpriteAnimator>(m_DefaultTexture);
	}

	void SpriteAnimator::UseDefaultTexture()
	{
		m_Mesh->Materials[0]->GetShader().GetLink("Texture") = m_DefaultTexture;
	}

	void SpriteAnimator::UseAnimation()
	{
		//m_Material->Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(m_CurrentAnimation.Animation.Columns, m_CurrentAnimation.Animation.Rows, m_CurrentAnimation.RepeatTime);
		//m_Material->Textures.Textures[0] = m_CurrentAnimation.Animation.Texture;
	}

}