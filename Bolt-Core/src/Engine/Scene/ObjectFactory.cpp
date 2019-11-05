#include "bltpch.h"
#include "ObjectFactory.h"
#include "Layer.h"
#include "Graphics/Resources/ResourceManager.h"
#include "Graphics/Resources/Meshes/Factories/GridFactory.h"
#include "Graphics/Resources/Meshes/Factories/LineFactory.h"

#include "Engine/CurrentContext.h"

namespace Bolt
{

	ObjectFactory::ObjectFactory() : ObjectFactory(*(Layer*)nullptr)
	{
		SceneManager& manager = CurrentContext::GetSceneManager();
		if (&manager != nullptr && &manager.CurrentScene() != nullptr)
		{
			SetCurrentLayer(CurrentContext::GetSceneManager().CurrentScene().GetCurrentLayer());
		}
	}

	ObjectFactory::ObjectFactory(Layer& layer)
		: m_CurrentLayer(&layer), m_CurrentParent(nullptr), m_Prefabs()
	{
	
	}

	ObjectFactory::ObjectFactory(GameObject* parent) 
		: m_CurrentLayer(parent->GetLayer()), m_CurrentParent(parent), m_Prefabs()
	{

	}

	Layer* ObjectFactory::CurrentLayer() const
	{
		return m_CurrentLayer;
	}

	GameObject* ObjectFactory::CurrentParent() const
	{
		return m_CurrentParent;
	}

	void ObjectFactory::SetCurrentLayer(Layer& layer)
	{
		m_CurrentLayer = &layer;
	}

	void ObjectFactory::SetCurrentParent(GameObject* parent)
	{
		m_CurrentParent = parent;
	}

	id_t ObjectFactory::AddPrefab(ObjectPrefab&& prefab)
	{
		id_t index = m_Prefabs.size();
		m_Prefabs.push_back(std::move(prefab));
		return index;
	}

	const ObjectPrefab* ObjectFactory::GetPrefab(id_t index) const
	{
		return &m_Prefabs[index];
	}

	ObjectPrefab* ObjectFactory::GetPrefab(id_t index)
	{
		return &m_Prefabs[index];
	}

	void ObjectFactory::ClearPrefabs()
	{
		m_Prefabs.clear();
	}

	GameObject* ObjectFactory::Instantiate(Transform transform) const
	{
		GameObject* object = GameObject::Instantiate(CurrentLayer(), std::move(transform));
		object->MakeChildOf(CurrentParent());
		return object;
	}

	GameObject* ObjectFactory::Instantiate(float x, float y, float z, const Quaternion& orientation, const Vector3f& scale) const
	{
		Transform t = Transform(Vector3f(x, y, z), orientation, scale);
		return Instantiate(std::move(t));
	}

	GameObject* ObjectFactory::Instantiate(const ObjectPrefab* prefab) const
	{
		return Instantiate(prefab, Transform());
	}

	GameObject* ObjectFactory::Instantiate(const ObjectPrefab* prefab, Transform transform) const
	{
		GameObject* object = GameObject::Instantiate(CurrentLayer(), prefab, std::move(transform));
		object->MakeChildOf(CurrentParent());
		return object;
	}

	GameObject* ObjectFactory::Instantiate(const ObjectPrefab* prefab, float x, float y, float z, const Quaternion& orientation, const Vector3f& scale) const
	{
		Transform t = Transform(Vector3f(x, y, z), orientation, scale);
		return Instantiate(prefab, std::move(t));
	}

	GameObject* ObjectFactory::Instantiate(std::unique_ptr<MeshRenderer>&& meshRenderer, Transform transform) const
	{
		GameObject* object = Instantiate(std::move(transform));
		object->Components().AddComponent(std::move(meshRenderer));
		return object;
	}

	GameObject* ObjectFactory::Instantiate(Mesh mesh, Transform transform) const
	{
		return Instantiate(std::make_unique<MeshRenderer>(std::move(mesh)), std::move(transform));
	}

	GameObject* ObjectFactory::Rectangle(float width, float height, const Color& color, Transform transform) const
	{
		return Rectangle(width, height, ResourceManager::Get().Materials().Default(color), std::move(transform));
	}

	GameObject* ObjectFactory::Rectangle(float width, float height, std::unique_ptr<Material>&& material, Transform transform) const
	{
		Mesh mesh;
		mesh.Models.push_back({ BasicModels::Get().Square(), Matrix4f::Scale(width, height, 1),{ 0 } });
		mesh.Materials.push_back(std::move(material));
		return Instantiate(std::move(mesh), std::move(transform));
	}

	GameObject* ObjectFactory::Image(float width, float height, const ResourcePtr<Texture2D>& image, Transform transform) const
	{
		GameObject* object = Rectangle(width, height, ResourceManager::Get().Materials().Texture(image), std::move(transform));
		return object;
	}

	GameObject* ObjectFactory::Ellipse(float width, float height, const Color& color, Transform transform) const
	{
		return Ellipse(width, height, ResourceManager::Get().Materials().Default(color), std::move(transform));
	}

	GameObject* ObjectFactory::Ellipse(float width, float height, std::unique_ptr<Material>&& material, Transform transform) const
	{
		Mesh mesh;
		mesh.Models.push_back({ BasicModels::Get().Circle(), Matrix4f::Scale(width / 2, height / 2, 1), { 0 } });
		mesh.Materials.push_back(std::move(material));
		return Instantiate(std::move(mesh), std::move(transform));
	}

	GameObject* ObjectFactory::Cuboid(float width, float height, float depth, const Color& color, Transform transform) const
	{
		return Cuboid(width, height, depth, ResourceManager::Get().Materials().DefaultLighting(color), std::move(transform));
	}

	GameObject* ObjectFactory::Cuboid(float width, float height, float depth, std::unique_ptr<Material>&& material, Transform transform) const
	{
		Mesh mesh;
		mesh.Models.push_back({ BasicModels::Get().Cube(), Matrix4f::Scale(width, height, depth),{ 0 } });
		mesh.Materials.push_back(std::move(material));
		return Instantiate(std::move(mesh), std::move(transform));
	}

	GameObject* ObjectFactory::Sphere(float radius, const Color& color, Transform transform) const
	{
		return Sphere(radius, ResourceManager::Get().Materials().DefaultLighting(color), std::move(transform));
	}

	GameObject* ObjectFactory::Sphere(float radius, std::unique_ptr<Material>&& material, Transform transform) const
	{
		Mesh mesh;
		mesh.Models.push_back({ BasicModels::Get().Sphere(), Matrix4f::Scale(radius), { 0 } });
		mesh.Materials.push_back(std::move(material));
		return Instantiate(std::move(mesh), std::move(transform));
	}

	GameObject* ObjectFactory::Grid(float width, float depth, int xVertices, int zVertices, const Color& color, Transform transform) const
	{
		return Grid(width, depth, xVertices, zVertices, ResourceManager::Get().Materials().DefaultLighting(color), std::move(transform));
	}

	GameObject* ObjectFactory::Grid(float width, float depth, int xVertices, int zVertices, std::unique_ptr<Material>&& material, Transform transform) const
	{
		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ ResourcePtr<Model>(BLT_NEW Model(GridFactory(width, depth, xVertices, zVertices)), true), Matrix4f::Identity(), { 0 } });
		mesh.Materials.push_back(std::move(material));
		return Instantiate(std::move(mesh), std::move(transform));
	}

	GameObject* ObjectFactory::Line(const Vector3f& p0, const Vector3f& p1, const Color& color) const
	{
		float distance = Vector3f::Distance(p0, p1);
		if (distance == 0.0f)
		{
			BLT_ASSERT(false, "Unable to create a line with 0 length");
			return nullptr;
		}
		Vector3f position = (p0 + p1) / 2.0f;

		Vector3f direction = p1 - p0;
		direction = direction.Normalize();
		Vector3f normal = Vector3f::Cross(Vector3f::Right(), direction);
		float dot = direction.Dot(Vector3f::Right());
		Quaternion orientation = Quaternion::FromAngleAxis(acos(dot), normal);

		Transform t(position, orientation, Vector3f(1, 1, 1));

		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ ResourcePtr<Model>(BLT_NEW Model(LineFactory(Vector3f::Right(), distance)), true), Matrix4f::Identity(),{ 0 } });
		mesh.Materials[0] = ResourceManager::Get().Materials().Default(color);

		return Instantiate(std::move(mesh), std::move(t));
	}

}