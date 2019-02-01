#include "Types.h"

#include "UIsurface.h"
#include "UIroot.h"

namespace Bolt
{

	UIsurface::UIsurface(float width, float height, const Color& color, Transform&& transform) : UIelement(),
		m_Width(width), m_Height(height), m_Transform(std::move(transform)), m_Material()
	{
		m_Material.BaseColor = color;
	}

	UIsurface::UIsurface(float width, float height, const Material& material, Transform&& transform) : UIelement(),
		m_Width(width), m_Height(height), m_Transform(std::move(transform)), m_Material(material)
	{
		
	}

	void UIsurface::SetUIroot(UIroot* root)
	{
		UIelement::SetUIroot(root);
		Mesh mesh;
		mesh.Models.push_back({ ObjectFactory::SquareModel(), Matrix4f::Scale(m_Width, m_Height, 1), { 0 } });
		mesh.Materials[0] = m_Material;
		m_Object->Components().AddComponent(std::make_unique<MeshRenderer>(std::move(mesh)));
		m_Object->transform() = std::move(m_Transform);
	}

}