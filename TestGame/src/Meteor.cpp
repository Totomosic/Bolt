#include "Meteor.h"
#include "RigidBody.h"

namespace MeteorGame
{

	Meteor::Meteor(float radius, MeteorType type) : Component(),
		Radius(radius), IsBroken(false), Type(type)
	{
	
	}

	void Meteor::Update()
	{
		if (gameObject()->transform().Position().y - Radius <= 100)
		{
			if (!IsBroken && Type == MeteorType::Default)
			{
				Break();
			}
			else if (Type == MeteorType::Flame)
			{
				CreateFire();
			}
			Destroy(gameObject(), 0.0f);
		}
	}

	void Meteor::Break()
	{
		int count = Random::NextInt(2, 5);
		for (int i = 0; i < count; i++)
		{
			float scale = Random::NextFloat(0.1f, 0.5f);
			GameObject* rock = GameObject::Instantiate(layer(), gameObject());
			Vector3f position = gameObject()->transform().Position();
			float radius = Radius * scale;
			position.y = 100 + radius + 5;
			rock->transform().SetLocalPosition(position);
			rock->transform().SetLocalScale(radius, radius, 1);

			RigidBody& rb = rock->Components().GetComponent<RigidBody>();
			rb.Velocity = Vector3f(Random::NextFloat(-200, 200), Random::NextFloat(6, 7) * Radius, 0);
			rb.zAngle = -rb.Velocity.x * 0.03f;

			Meteor& m = rock->Components().GetComponent<Meteor>();
			m.Radius = radius;
			m.IsBroken = true;
		}
	}

	void Meteor::CreateFire()
	{
		Texture2D* explosionSheet = ResourceManager::Get<Texture2D>("Explosion2");
		Model* model = ResourceManager::Get<Model>("Square");
		GameObject* fire = GameObject::Instantiate(layer());
		float scaleFactor = 2.0f;
		Vector3f position = gameObject()->transform().Position();
		position.y = 100 + Radius * scaleFactor;
		fire->transform().SetLocalPosition(position);

		float animTime = 0.5f;
		Mesh mesh;
		mesh.Models.push_back({ model, Matrix4f::Scale(Radius * 2 * 1.5f * scaleFactor, Radius * 2 * 1.5f * scaleFactor, 1), { 0 } });
		mesh.Materials[0].Shader = Shader::DefaultTexture();
		mesh.Materials[0].Textures.Textures.push_back(explosionSheet);
		mesh.Materials[0].Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(4, 4, animTime);

		fire->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
		Destroy(fire, animTime);
	}

	std::unique_ptr<Component> Meteor::Clone() const
	{
		std::unique_ptr<Meteor> m = std::make_unique<Meteor>(Radius, Type);
		m->IsBroken = IsBroken;
		return std::move(m);
	}

}