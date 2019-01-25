#pragma once
#include "TileTransform.h"
#include "CharacterAnimator.h"
#include "SpriteAnimator.h"

namespace DND
{

	class TileMotion : public Component
	{
	private:
		Tile m_TargetTile;
		float m_Speed;
		bool m_Seeking;
		TileTransform* m_Transform;
		CharacterAnimator* m_Animator;

	public:
		TileMotion(float speed);

		const Tile& TargetTile() const;
		float CurrentSpeed() const;
		bool IsMoving() const;
		bool HasAnimator() const;

		void Start() override;
		void Update() override;

		void SetTargetTile(const Tile& tile);
		void SetCurrentSpeed(float speed);
		void FindAnimator();

		std::unique_ptr<Component> Clone() const override;

	};

}