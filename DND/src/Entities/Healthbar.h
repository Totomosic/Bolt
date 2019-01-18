#pragma once
#include "Characters/CharacterStats.h"

namespace DND
{

	class Healthbar : public Component
	{
	private:
		Layer* m_Layer;
		Vector3f m_PositionOffset;
		float m_BarLength;
		GameObject* m_Object;

	public:
		Healthbar(Layer* layer, const Vector3f& positionOffset);

		void Start() override;
		void End() override;

		std::unique_ptr<Component> Clone() const override;

	private:
		void SetBarSize(float size, float maxSize) const;

	};

}