#include "Types.h"

#include "MotionEngine.h"
#include "../GameObject.h"
#include "Core/Time/Time.h"

namespace Bolt
{

	MotionEngine::MotionEngine() : MotionEngine(Vector3f(0.0f), Vector3f(0.0f))
	{
	
	}

	MotionEngine::MotionEngine(const Vector3f& velocity, const Vector3f& acceleration) : Component(),
		Velocity(velocity), Acceleration(acceleration)
	{
	
	}

	void MotionEngine::Update()
	{
		Velocity += Acceleration * Time::Get().RenderingTimeline().DeltaTime();
		m_GameObject->transform().Translate(Velocity * Time::Get().RenderingTimeline().DeltaTime());
	}

	std::unique_ptr<Component> MotionEngine::Clone() const
	{
		return std::make_unique<MotionEngine>(Velocity, Acceleration);
	}

}