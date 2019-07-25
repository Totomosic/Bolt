#pragma once
#include "MotionController.h"
#include "Actions.h"

namespace DND
{

	class PlayerMoveAction : public CharacterAction
	{
	private:
		Tile m_Delta;

	public:
		PlayerMoveAction(const Tile& delta);

		ActionResult Execute(GameObject* object, MotionController& motionController) override;

	};

	class PlayerController : public Component
	{
	private:
		MotionController m_MotionController;
		ActionManager m_Actions;
		
		Keycode m_LastPressed;

	public:
		PlayerController();

		bool CanMove() const;
		bool CanPerformActions() const;

		void Start() override;
		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	private:
		Keycode GetLastPressedKey() const;
		std::unique_ptr<CharacterAction> GetAction(Keycode key) const;
		void PushAction(std::unique_ptr<CharacterAction>&& action);

	};

}