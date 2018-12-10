#include "Types.h"
#include "UIsurface.h"
#include "..\ObjectFactory.h"

namespace Bolt
{

	UIsurface::UIsurface(const Vector2f& size, Transform&& transform) : UIsurface(size, Material(), std::move(transform))
	{
		m_Material.BaseColor = Color(0, 0, 0, 0);
	}

	UIsurface::UIsurface(const Vector2f& size, Material material, Transform&& transform) : UIelement(),
		m_Size(size), m_Material(material), m_Transform(std::move(transform))
	{
	
	}

	UIsurface::UIsurface(const Vector2f& size, const Color& color, Transform&& transform) : UIsurface(size, Material(), std::move(transform))
	{
		m_Material.BaseColor = color;
	}

	UIsurface::UIsurface(const Vector2f& size, const ResourcePtr<const Texture2D>& texture, Transform&& transform) : UIsurface(size, Material(), std::move(transform))
	{
		m_Material.BaseColor = Color::White;
		m_Material.Shader = Shader::DefaultTexture();
		m_Material.Textures.Textures.push_back(texture);
	}

	const Vector2f& UIsurface::Size() const
	{
		return m_Size;
	}

	const Material& UIsurface::GetMaterial() const
	{
		return m_Material;
	}

	void UIsurface::SetSize(const Vector2f& size)
	{
		if (m_Size.x > 0 && m_Size.y > 0)
		{
			Mesh& mesh = m_GameObject->Components().GetComponent<MeshRenderer>().Mesh;
			mesh.Models[0].Transform = Matrix4f::Scale(m_Size.x, m_Size.y, 1.0f);
		}
		else
		{
			Mesh mesh;
			mesh.Models.push_back({ ResourcePtr<const Model>(new Model(RectangleFactory(1.0f, 1.0f, Color::White)), true), Matrix4f::Scale(m_Size.x, m_Size.y, 1.0f), { 0 } });
			mesh.Materials[0] = m_Material;
		}
		m_Size = size;
	}

	void UIsurface::SetMaterial(Material material)
	{
		m_Material = std::move(material);
		if (m_Size.x > 0 && m_Size.y > 0)
		{
			Mesh& mesh = m_GameObject->Components().GetComponent<MeshRenderer>().Mesh;
			mesh.Materials[0] = m_Material;
		}
	}

	void UIsurface::CreateGameObject()
	{
		ObjectFactory factory(Parent()->Object()->GetLayer());
		if (m_Size.x > 0 && m_Size.y > 0)
		{
			SetGameObject(factory.Rectangle(m_Size.x, m_Size.y, m_Material, std::move(m_Transform)));
		}
		else
		{
			SetGameObject(factory.Instantiate(std::move(m_Transform)));
		}
	}

}