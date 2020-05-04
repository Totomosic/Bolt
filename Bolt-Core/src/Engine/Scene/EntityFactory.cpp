#include "bltpch.h"
#include "EntityFactory.h"

#include "Layer.h"
#include "Components/Camera.h"
#include "Components/Transform.h"

#include "Graphics/Assets/AssetManager.h"
#include "Graphics/Assets/Meshes/Factories/GridFactory.h"

namespace Bolt
{

	EntityFactory::EntityFactory(EntityManager& manager)
		: m_Manager(&manager)
	{
	}

	EntityFactory::EntityFactory(Layer& layer) : EntityFactory(layer.Entities())
	{
	}

	EntityHandle EntityFactory::Empty() const
	{
		return m_Manager->Create();
	}

	EntityHandle EntityFactory::CreateTransform(Transform transform) const
	{
		EntityHandle entity = Empty();
		entity.Assign<Transform>(std::move(transform));
		return entity;
	}

	EntityHandle EntityFactory::Camera(const Matrix4f& projection, Transform transform) const
	{
		EntityHandle camera = Empty();
		camera.Assign<Bolt::Camera>(projection);
		camera.Assign<Transform>(std::move(transform));
		return camera;
	}

	EntityHandle EntityFactory::Camera(Transform transform) const
	{
		return Camera(Matrix4f::Identity(), std::move(transform));
	}

	EntityHandle EntityFactory::CreateMesh(Mesh mesh, Transform transform) const
	{
		EntityHandle entity = Empty();
		entity.Assign<Mesh>(std::move(mesh));
		entity.Assign<Transform>(std::move(transform));
		return entity;
	}

	EntityHandle EntityFactory::CreateMesh(AssetHandle<const Model> model, std::unique_ptr<Material>&& material, const Matrix4f& transformMatrix, Transform transform) const
	{
		Mesh mesh;
		mesh.Models.push_back({ std::move(model), transformMatrix, { 0 } });
		mesh.Materials.push_back(std::move(material));
		return CreateMesh(std::move(mesh), std::move(transform));
	}

	EntityHandle EntityFactory::Rectangle(float width, float height, const Color& color, Transform transform) const
	{
		return Rectangle(width, height, AssetManager::Get().Materials().Default(color), std::move(transform));
	}

	EntityHandle EntityFactory::Rectangle(float width, float height, std::unique_ptr<Material>&& material, Transform transform) const
	{
		return CreateMesh(AssetManager::Get().Models().Square(), std::move(material), Matrix4f::Scale(width, height, 1), std::move(transform));
	}

	EntityHandle EntityFactory::Image(float width, float height, AssetHandle<Texture2D> image, Transform transform) const
	{
		return Rectangle(width, height, AssetManager::Get().Materials().Texture(std::move(image)), std::move(transform));
	}

	EntityHandle EntityFactory::Ellipse(float width, float height, const Color& color, Transform transform) const
	{
		return Ellipse(width, height, AssetManager::Get().Materials().Default(color), std::move(transform));
	}

	EntityHandle EntityFactory::Ellipse(float width, float height, std::unique_ptr<Material>&& material, Transform transform) const
	{
		return CreateMesh(AssetManager::Get().Models().Circle(), std::move(material), Matrix4f::Scale(width / 2.0f, height / 2.0f, 1), std::move(transform));
	}

	EntityHandle EntityFactory::EllipseImage(float width, float height, AssetHandle<Texture2D> image, Transform transform) const
	{
		return Ellipse(width, height, AssetManager::Get().Materials().Texture(std::move(image)), std::move(transform));
	}

	EntityHandle EntityFactory::Circle(float radius, const Color& color, Transform transform) const
	{
		return Ellipse(radius * 2, radius * 2, color, std::move(transform));
	}

	EntityHandle EntityFactory::Circle(float radius, std::unique_ptr<Material>&& material, Transform transform) const
	{
		return Ellipse(radius * 2, radius * 2, std::move(material), std::move(transform));
	}

	EntityHandle EntityFactory::Text(const std::string& text, AssetHandle<Font> font, const Color& color, Transform transform, AlignH horizontal, AlignV vertical) const
	{
		ModelData data = TextFactory(text, font, Color::White, horizontal, vertical).GenerateVertices();
		AssetHandle<const Model> model = AssetHandle<const Model>(new Model(std::move(data), false), true);
		return CreateMesh(std::move(model), AssetManager::Get().Materials().Font(font, color), Matrix4f::Identity(), std::move(transform));
	}

	EntityHandle EntityFactory::Text(const std::string& text, const Color& color, Transform transform, AlignH horizontal, AlignV vertical) const
	{
		return Text(text, AssetManager::Get().Fonts().Default(), color, std::move(transform), horizontal, vertical);
	}

	EntityHandle EntityFactory::Cuboid(float width, float height, float depth, const Color& color, Transform transform) const
	{
		return Cuboid(width, height, depth, AssetManager::Get().Materials().DefaultLighting(color), std::move(transform));
	}

	EntityHandle EntityFactory::Cuboid(float width, float height, float depth, std::unique_ptr<Material>&& material, Transform transform) const
	{
		return CreateMesh(AssetManager::Get().Models().Cube(), std::move(material), Matrix4f::Scale(width, height, depth), std::move(transform));
	}

	EntityHandle EntityFactory::Ellipsoid(float width, float height, float depth, const Color& color, Transform transform) const
	{
		return Ellipsoid(width, height, depth, AssetManager::Get().Materials().DefaultLighting(color), std::move(transform));
	}

	EntityHandle EntityFactory::Ellipsoid(float width, float height, float depth, std::unique_ptr<Material>&& material, Transform transform) const
	{
		return CreateMesh(AssetManager::Get().Models().Sphere(), std::move(material), Matrix4f::Scale(width / 2.0f, height / 2.0f, depth / 2.0f), std::move(transform));
	}

	EntityHandle EntityFactory::Sphere(float radius, const Color& color, Transform transform) const
	{
		return Ellipsoid(radius * 2, radius * 2, radius * 2, color, std::move(transform));
	}

	EntityHandle EntityFactory::Sphere(float radius, std::unique_ptr<Material>&& material, Transform transform) const
	{
		return Ellipsoid(radius * 2, radius * 2, radius * 2, std::move(material), std::move(transform));
	}

	EntityHandle EntityFactory::Grid(float width, float height, int xVertices, int yVertices, const Color& color, Transform transform) const
	{
		return Grid(width, height, xVertices, yVertices, AssetManager::Get().Materials().DefaultLighting(color), std::move(transform));
	}

	EntityHandle EntityFactory::Grid(float width, float height, int xVertices, int yVertices, std::unique_ptr<Material>&& material, Transform transform) const
	{
		ModelData data = GridFactory(1.0f, 1.0f, xVertices, yVertices, Color::White).GenerateVertices();
		AssetHandle<const Model> model = AssetHandle<const Model>(new Model(std::move(data), true), true);
		return CreateMesh(std::move(model), std::move(material), Matrix4f::Scale(width, 1.0f, height), std::move(transform));
	}

}