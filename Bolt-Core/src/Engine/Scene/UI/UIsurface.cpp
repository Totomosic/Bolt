#include "bltpch.h"

#include "UIsurface.h"
#include "UIroot.h"
#include "Graphics/Resources/ResourceManager.h"

namespace Bolt
{

	UIsurface::UIsurface(float width, float height, const Color& color, Transform&& transform) : UIelement(),
		m_Width(width), m_Height(height), m_Transform(std::move(transform)), m_Material(ResourceManager::Get().Materials().Default(color))
	{
		
	}

	UIsurface::UIsurface(float width, float height, std::unique_ptr<Material>&& material, Transform&& transform) : UIelement(),
		m_Width(width), m_Height(height), m_Transform(std::move(transform)), m_Material(std::move(material))
	{
		
	}

	void UIsurface::SetUIroot(UIroot* root)
	{
		UIelement::SetUIroot(root);
		Mesh mesh;
		mesh.Models.push_back({ BasicModels::Get().Square(), Matrix4f::Scale(m_Width, m_Height, 1), { 0 } });
		if (mesh.Materials.size() < 1)
		{
			mesh.Materials.resize(1);
		}
		mesh.Materials[0] = std::move(m_Material);
		m_Object->Components().AddComponent<MeshRenderer>(std::move(mesh));
		m_Object->transform() = std::move(m_Transform);
	}

}