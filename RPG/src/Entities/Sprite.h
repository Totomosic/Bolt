#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace RPG
{

	class Sprite
	{
	protected:
		Vector2f m_Position;
		Vector2f m_Size;
		const Texture2D* m_Texture;

		id_t m_LayerId;
		GameObject* m_Object;
		bool m_IsCreated;
		
	public:
		Sprite();
		Sprite(const Vector2f& position, const Vector2f& size, const Texture2D* texture = nullptr, id_t layerId = 0);
		Sprite(const Sprite& other) = delete;
		Sprite& operator=(const Sprite& other) = delete;
		Sprite(Sprite&& other) = default;
		Sprite& operator=(Sprite&& other) = default;
		virtual ~Sprite();

		Vector2f Position() const;
		const Vector2f& Size() const;
		const Texture2D* Texture() const;
		GameObject* Object() const;
		bool IsCreated() const;

		void SetPosition(const Vector2f& position);
		void Translate(float x, float y);
		void SetTexture(const Texture2D* texture);
		virtual void Update();

		virtual GameObject* Create();
		virtual void Destroy();

	};

}