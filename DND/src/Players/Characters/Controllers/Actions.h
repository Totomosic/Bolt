#pragma once
#include "MotionController.h"

namespace DND
{

	struct ActionResult
	{
	public:
		bool IsCompleted = true;

	};
	
	class CharacterAction
	{
	public:
		virtual ActionResult Execute(GameObject* object, MotionController& motionController) = 0;

	};

	using ActionManager = std::deque<std::unique_ptr<CharacterAction>>;

}