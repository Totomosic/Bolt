#pragma once
#include "TileMotion.h"
#include "CharacterAnimator.h"

namespace DND
{

	class PlayerController : public Component
	{
	public:
		using PlayerAction = std::function<void(GameObject*)>;
		static constexpr int MAX_QUEUED_ACTIONS = 2;

	private:
		Keycode m_Left;
		Keycode m_Up;
		Keycode m_Right;
		Keycode m_Down;

		Keycode m_LastPressed;

		std::deque<PlayerAction> m_Actions;
		bool m_CanMove;

	public:
		PlayerController();

		inline bool IsFrozen() const { return !m_CanMove; }
		inline void Freeze() { SetFreeze(true); }
		inline void Unfreeze() { SetFreeze(false); }
		inline void SetFreeze(bool isFrozen) { m_CanMove = !isFrozen; }

		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	private:
		Keycode TestPressedKeys() const;
		Tile GetTileOffset(Keycode key) const;

		void QueueAction(const PlayerAction& action);

	};

}