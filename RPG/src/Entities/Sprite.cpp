#include "Sprite.h"

namespace RPG
{

	Sprite::Sprite() : Sprite(Vector2f(), Vector2f(0, 0), nullptr, 0)
	{
	
	}

	Sprite::Sprite(const Vector2f& position, const Vector2f& size, const Texture2D* texture, id_t layerId)
		: m_Position(position), m_Size(size), m_Texture(texture), m_Object(nullptr), m_LayerId(layerId), m_IsCreated(false)
	{
		
	}

	Sprite::~Sprite()
	{

	}

	Vector2f Sprite::Position() const
	{
		return m_Position;
	}

	const Vector2f& Sprite::Size() const
	{
		return m_Size;
	}

	const Texture2D* Sprite::Texture() const
	{
		return m_Texture;
	}

	GameObject* Sprite::Object() const
	{
		return m_Object;
	}

	bool Sprite::IsCreated() const
	{
		return m_IsCreated;
	}

	void Sprite::SetPosition(const Vector2f& position)
	{
		m_Position = position;
		m_Object->transform().SetLocalPosition(position.x, position.y, m_Object->transform().Position().z);
	}

	void Sprite::Translate(float x, float y)
	{
		SetPosition(m_Position + Vector2f(x, y));
	}

	void Sprite::SetTexture(const Texture2D* texture)
	{
		m_Texture = texture;
		m_Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Textures[0] = texture;
	}

	void Sprite::Update()
	{
	
	}

	GameObject* Sprite::Create()
	{
		Layer* layer = &SceneManager::CurrentScene().GetLayer(m_LayerId);
		m_Object = GameObject::Instantiate(layer);
		m_Object->transform().SetLocalPosition(m_Position.x, m_Position.y, -80);

		Mesh mesh;
		mesh.Models.push_back({ ResourceManager::Get<Model>("Sprite_Model"), Matrix4f::Scale(m_Size.x, m_Size.y, 1.0f), { 0 } });
		if (m_Texture != nullptr)
		{
			mesh.Materials[0].Shader = Shader::DefaultTexture();
			mesh.Materials[0].Textures.Textures.push_back(m_Texture);
		}
		m_Object->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
		m_IsCreated = true;
		return m_Object;
	}

	void Sprite::Destroy()
	{
		m_Object->GetLayer()->RemoveGameObject(m_Object);
		m_IsCreated = false;
	}

}