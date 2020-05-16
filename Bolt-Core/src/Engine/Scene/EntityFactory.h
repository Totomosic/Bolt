#pragma once
#include "EntityManager.h"
#include "Components/Transform.h"
#include "BoltLib/Color.h"

#include "Graphics/Assets/Meshes/Materials/Material.h"
#include "Graphics/Assets/Meshes/Model.h"
#include "Graphics/Assets/Meshes/Factories/TextFactory.h"

namespace Bolt
{

	class Layer;

	class BLT_API EntityFactory
	{
	private:
		EntityManager* m_Manager;

	public:
		EntityFactory(EntityManager& manager);
		EntityFactory(Layer& layer);

		EntityHandle Empty() const;
		EntityHandle CreateTransform(Transform transform = Transform()) const;

		EntityHandle Camera(const Matrix4f& projection, Transform transform = Transform()) const;
		EntityHandle Camera(Transform transform = Transform()) const;

		EntityHandle CreateModel(Model model, Transform transform = Transform()) const;
		EntityHandle CreateModel(AssetHandle<Mesh> mesh, std::unique_ptr<Material>&& material, const Matrix4f& transformMatrix = Matrix4f::Identity(), Transform transform = Transform()) const;

		EntityHandle Rectangle(float width, float height, const Color& color, Transform transform = Transform()) const;
		EntityHandle Rectangle(float width, float height, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;
		EntityHandle Image(float width, float height, AssetHandle<Texture2D> image, Transform transform = Transform()) const;
		EntityHandle Ellipse(float width, float height, const Color& color, Transform transform = Transform()) const;
		EntityHandle Ellipse(float width, float height, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;
		EntityHandle EllipseImage(float width, float height, AssetHandle<Texture2D> image, Transform transform = Transform()) const;
		EntityHandle Circle(float radius, const Color& color, Transform transform = Transform()) const;
		EntityHandle Circle(float radius, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;

		EntityHandle Text(const std::string& text, AssetHandle<Font> font, const Color& color, Transform transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center) const;
		EntityHandle Text(const std::string& text, const Color& color, Transform transform = Transform(), AlignH horizontal = AlignH::Center, AlignV vertical = AlignV::Center) const;

		EntityHandle Cuboid(float width, float height, float depth, const Color& color, Transform transform = Transform()) const;
		EntityHandle Cuboid(float width, float height, float depth, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;
		EntityHandle Ellipsoid(float width, float height, float depth, const Color& color, Transform transform = Transform()) const;
		EntityHandle Ellipsoid(float width, float height, float depth, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;
		EntityHandle Sphere(float radius, const Color& color, Transform transform = Transform()) const;
		EntityHandle Sphere(float radius, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;
		EntityHandle Grid(float width, float height, int xVertices, int yVertices, const Color& color, Transform transform = Transform()) const;
		EntityHandle Grid(float width, float height, int xVertices, int yVertices, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;
	};

}