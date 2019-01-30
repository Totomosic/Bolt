#include "bltpch.h"
#include "Healthbar.h"
#include "../GameManager.h"

namespace DND
{

	Healthbar::Healthbar(Layer* layer, const Vector3f& positionOffset) : Component(),
		m_Layer(layer), m_PositionOffset(positionOffset), m_BarLength(50), m_Object(nullptr)
	{
	
	}

	void Healthbar::Start()
	{
		ObjectFactory factory(*m_Layer);
		m_Object = factory.Rectangle(1, 5, Color::Red, Transform(m_PositionOffset));
		m_Object->MakeChildOf(gameObject());
		
		StatsComponent& c = gameObject()->Components().GetComponent<StatsComponent>();
		float healthProp = c.Stats().CurrentHealth / (float)c.Stats().MaxHealth;
		SetBarSize(m_BarLength * healthProp, m_BarLength);

		c.OnStatsChanged.Subscribe([this](id_t listenerId, UpdatedStatsEvent& e)
		{
			if (e.StatsDelta.CurrentHealth != 0 || e.StatsDelta.MaxHealth != 0)
			{
				float healthProp = e.Stats.CurrentHealth / (float)e.Stats.MaxHealth;
				SetBarSize(m_BarLength * healthProp, m_BarLength);

				if (e.Object == GameManager::Get().Players().LocalPlayerObject() && e.Stats.CurrentHealth <= 0)
				{
					Time::RenderingTimeline().AddFunction(0.25, []()
					{
						GameManager::Get().Exit();
					});
				}
				else if (e.Stats.CurrentHealth <= 0)
				{
					id_t networkId = gameObject()->Components().GetComponent<NetworkIdentity>().NetworkId;
					GameManager::Get().Network().Objects().DestroyObject(networkId);
				}
			}
			return false;
		});
	}

	void Healthbar::End()
	{
		Destroy(m_Object);
	}

	std::unique_ptr<Component> Healthbar::Clone() const
	{
		return std::make_unique<Healthbar>(m_Layer, m_PositionOffset);
	}

	void Healthbar::SetBarSize(float size, float maxSize) const
	{
		m_Object->transform().SetLocalScale(std::clamp(size, 0.01f, 1.0f * maxSize), 1, 1);
		m_Object->transform().SetLocalPosition(m_PositionOffset + Vector3f(-(maxSize - size) / 2.0f, 0, 0));
	}

}