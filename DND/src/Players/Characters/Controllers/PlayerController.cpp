#include "bltpch.h"
#include "PlayerController.h"

namespace DND
{

	PlayerMoveAction::PlayerMoveAction(const Tile& delta) : CharacterAction(),
		m_Delta(delta)
	{
	
	}

	ActionResult PlayerMoveAction::Execute(GameObject* object, MotionController& motionController)
	{
		Tile currentTile = object->Components().GetComponent<TileTransform>().BottomLeftTile();
		motionController.MoveToBottomLeftTile(currentTile + m_Delta);
		ActionResult result;
		result.IsCompleted = true;
		return result;
	}

	PlayerController::PlayerController()
		: m_MotionController(), m_Actions(), m_LastPressed(Keycode::W)
	{
		
	}

	bool PlayerController::CanMove() const
	{
		return !gameObject()->Components().GetComponent<TileMotion>().IsSeeking();
	}

	bool PlayerController::CanPerformActions() const
	{
		return !gameObject()->Components().GetComponent<TileMotion>().IsSeeking();
	}

	void PlayerController::Start()
	{
		m_MotionController.Initialize(gameObject());
	}

	void PlayerController::Update()
	{
		m_LastPressed = GetLastPressedKey();
		if (CanMove())
		{
			if (Input::Get().KeyDown(m_LastPressed))
			{
				PushAction(GetAction(m_LastPressed));
			}
			else if (Input::Get().KeyDown(Keycode::W))
			{
				PushAction(GetAction(Keycode::W));
			}
			else if (Input::Get().KeyDown(Keycode::S))
			{
				PushAction(GetAction(Keycode::S));
			}
			else if (Input::Get().KeyDown(Keycode::A))
			{
				PushAction(GetAction(Keycode::A));
			}
			else if (Input::Get().KeyDown(Keycode::D))
			{
				PushAction(GetAction(Keycode::D));
			}
		}

		if (!m_Actions.empty() && CanPerformActions())
		{
			ActionResult result = m_Actions.front()->Execute(gameObject(), m_MotionController);
			if (result.IsCompleted)
			{
				m_Actions.pop_front();
			}
		}
	}

	std::unique_ptr<Component> PlayerController::Clone() const
	{
		return std::make_unique<PlayerController>();
	}

	Keycode PlayerController::GetLastPressedKey() const
	{
		if (Input::Get().KeyPressed(Keycode::W))
		{
			return Keycode::W;
		}
		if (Input::Get().KeyPressed(Keycode::S))
		{
			return Keycode::S;
		}
		if (Input::Get().KeyPressed(Keycode::A))
		{
			return Keycode::A;
		}
		if (Input::Get().KeyPressed(Keycode::D))
		{
			return Keycode::D;
		}
		return m_LastPressed;
	}

	std::unique_ptr<CharacterAction> PlayerController::GetAction(Keycode key) const
	{
		switch (key)
		{
		case Keycode::W:
			return std::make_unique<PlayerMoveAction>(Tile(0, 1));
		case Keycode::S:
			return std::make_unique<PlayerMoveAction>(Tile(0, -1));
		case Keycode::A:
			return std::make_unique<PlayerMoveAction>(Tile(-1, 0));
		case Keycode::D:
			return std::make_unique<PlayerMoveAction>(Tile(1, 0));
		}
		return nullptr;
	}

	void PlayerController::PushAction(std::unique_ptr<CharacterAction>&& action)
	{
		if (m_Actions.size() < 2)
		{
			m_Actions.push_back(std::move(action));
		}
	}

}