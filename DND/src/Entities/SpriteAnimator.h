#pragma once
#include "bltpch.h"

namespace DND
{

	struct SpriteAnimation
	{
	public:
		ResourcePtr<Texture2D> Animation;
		int Columns;
		int Rows;
	};

	class SpriteAnimator : public Component
	{
	public:
		struct AnimationInfo
		{
		public:
			SpriteAnimation Animation;
			float RemainingTime;
		};

	private:
		AnimationInfo m_CurrentAnimation;
		ResourcePtr<Texture2D> m_DefaultTexture;

		Material* m_Material;

	public:
		SpriteAnimator(ResourcePtr<Texture2D> defaultTexture);

		const AnimationInfo& CurrentAnimation() const;
		bool IsPlayingAnimation() const;
		const ResourcePtr<Texture2D>& DefaultTexture() const;

		void Start() override;
		void Update() override;

		void PlayAnimation(SpriteAnimation animation, float time);
		void PlayAnimationUntilStopped(SpriteAnimation animation, float repeatTime);
		void StopAnimation();
		void SetDefaultTexture(ResourcePtr<Texture2D> defaultTexture);

		std::unique_ptr<Component> Clone() const override;

	};

}