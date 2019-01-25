#include "Types.h"
#include "TriggerComponent.h"

namespace Bolt
{

	TriggerComponent::TriggerComponent(TriggerComponent::TriggerFunc onStart, TriggerComponent::TriggerFunc onUpdate, TriggerComponent::TriggerFunc onLateUpdate, TriggerComponent::TriggerFunc onEnd) : Component(),
		OnStart(std::move(onStart)), OnUpdate(std::move(onUpdate)), OnLateUpdate(std::move(onLateUpdate)), OnEnd(std::move(onEnd))
	{
	
	}

	void TriggerComponent::Start()
	{
		if (OnStart)
		{
			OnStart(gameObject());
		}
	}

	void TriggerComponent::Update()
	{
		if (OnUpdate)
		{
			OnUpdate(gameObject());
		}
	}

	void TriggerComponent::LateUpdate()
	{
		if (OnLateUpdate)
		{
			OnLateUpdate(gameObject());
		}
	}

	void TriggerComponent::End()
	{
		if (OnEnd)
		{
			OnEnd(gameObject());
		}
	}

	std::unique_ptr<Component> TriggerComponent::Clone() const
	{
		return std::make_unique<TriggerComponent>(OnStart, OnUpdate, OnLateUpdate, OnEnd);
	}

}