#pragma once
#include "Component.h"

namespace Bolt
{

	class BLT_API TriggerComponent : public Component
	{
	public:
		using TriggerFunc = std::function<void(GameObject*)>;

	public:
		TriggerFunc OnStart;
		TriggerFunc OnUpdate;
		TriggerFunc OnLateUpdate;
		TriggerFunc OnEnd;

	public:
		TriggerComponent(TriggerFunc onStart = TriggerFunc(), TriggerFunc onUpdate = TriggerFunc(), TriggerFunc onLateUpdate = TriggerFunc(), TriggerFunc onEnd = TriggerFunc());

		void Start() override;
		void Update() override;
		void LateUpdate() override;
		void End() override;

		std::unique_ptr<Component> Clone() const override;

	};

}