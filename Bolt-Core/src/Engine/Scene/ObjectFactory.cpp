#include "Types.h"

#include "ObjectFactory.h"
#include "Layer.h"

namespace Bolt
{

	ResourcePtr<Model> ObjectFactory::s_RectangleModel = nullptr;
	ResourcePtr<Model> ObjectFactory::s_EllipseModel = nullptr;
	ResourcePtr<Model> ObjectFactory::s_CuboidModel = nullptr;

	ObjectFactory::ObjectFactory() : ObjectFactory((Layer*)nullptr)
	{
	
	}

	ObjectFactory::ObjectFactory(Layer* layer)
		: m_CurrentLayer(layer), m_CurrentParent(nullptr), m_Prefabs()
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

	void ObjectFactory::SetCurrentLayer(Layer* layer)
	{
		m_CurrentLayer = layer;
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
		GameObject* object = GameObject::Instantiate(CurrentLayer(), prefab);
		object->MakeChildOf(CurrentParent());
		return object;
	}

	GameObject* ObjectFactory::Instantiate(const ObjectPrefab* prefab, Transform transform) const
	{
		GameObject* object = Instantiate(prefab);
		object->transform() = std::move(transform);
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
		return Instantiate(std::make_unique<MeshRenderer>(mesh), std::move(transform));
	}

	GameObject* ObjectFactory::Rectangle(float width, float height, const Color& color, Transform transform) const
	{
		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ s_RectangleModel.Get(), Matrix4f::Scale(width, height, 1), { 0 } });
		mesh.Materials[0].BaseColor = color;
		mesh.Materials[0].Shader = Shader::DefaultColor();
		return Instantiate(mesh, std::move(transform));
	}

	GameObject* ObjectFactory::Rectangle(float width, float height, const Material& material, Transform transform) const
	{
		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ s_RectangleModel.Get(), Matrix4f::Scale(width, height, 1),{ 0 } });
		mesh.Materials[0] = material;
		return Instantiate(mesh, std::move(transform));
	}

	GameObject* ObjectFactory::Image(float width, float height, const ResourcePtr<const Texture2D>& image, Transform transform) const
	{
		GameObject* object = Rectangle(width, height, Color::White, std::move(transform));
		Mesh& mesh = object->Components().GetComponent<MeshRenderer>().Mesh;
		mesh.Materials[0].Shader = Shader::DefaultTexture();
		mesh.Materials[0].Textures.Textures.push_back(image);
		return object;
	}

	GameObject* ObjectFactory::Ellipse(float width, float height, const Color& color, Transform transform) const
	{
		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ s_EllipseModel.Get(), Matrix4f::Scale(width / 2, height / 2, 1), { 0 } });
		mesh.Materials[0].BaseColor = color;
		mesh.Materials[0].Shader = Shader::DefaultColor();
		return Instantiate(mesh, std::move(transform));
	}

	GameObject* ObjectFactory::Ellipse(float width, float height, const Material& material, Transform transform) const
	{
		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ s_EllipseModel.Get(), Matrix4f::Scale(width / 2, height / 2, 1), { 0 } });
		mesh.Materials[0] = material;
		return Instantiate(mesh, std::move(transform));
	}

	GameObject* ObjectFactory::Cuboid(float width, float height, float depth, const Color& color, Transform transform) const
	{
		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ s_CuboidModel.Get(), Matrix4f::Scale(width, height, depth),{ 0 } });
		mesh.Materials[0].BaseColor = color;
		mesh.Materials[0].Shader = Shader::LightingColor();
		return Instantiate(mesh, std::move(transform));
	}

	GameObject* ObjectFactory::Cuboid(float width, float height, float depth, const Material& material, Transform transform) const
	{
		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ s_EllipseModel.Get(), Matrix4f::Scale(width, height, depth),{ 0 } });
		mesh.Materials[0] = material;
		return Instantiate(mesh, std::move(transform));
	}

	GameObject* ObjectFactory::Grid(float width, float depth, int xVertices, int zVertices, const Color& color, Transform transform) const
	{
		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ ResourcePtr<Model>(new Model(GridFactory(width, depth, xVertices, zVertices)), true), Matrix4f::Identity(), { 0 } });
		mesh.Materials[0].BaseColor = color;
		mesh.Materials[0].Shader = Shader::LightingColor();
		return Instantiate(mesh, std::move(transform));
	}

	GameObject* ObjectFactory::Grid(float width, float depth, int xVertices, int zVertices, const Material& material, Transform transform) const
	{
		Mesh mesh;
		// TODO: CHANGE
		mesh.Models.push_back({ ResourcePtr<Model>(new Model(GridFactory(width, depth, xVertices, zVertices)), true), Matrix4f::Identity(), { 0 } });
		mesh.Materials[0] = material;
		return Instantiate(mesh, std::move(transform));
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
		mesh.Models.push_back({ ResourcePtr<Model>(new Model(LineFactory(Vector3f::Right(), distance)), true), Matrix4f::Identity(),{ 0 } });
		mesh.Materials[0].BaseColor = color;

		return Instantiate(mesh, std::move(t));
	}

	void ObjectFactory::Initialize()
	{
		s_RectangleModel = ResourcePtr<Model>(new Model(RectangleFactory(1, 1)), true);
		s_EllipseModel = ResourcePtr<Model>(new Model(EllipseFactory(2, 2)), true);
		s_CuboidModel = ResourcePtr<Model>(new Model(CuboidFactory(1, 1)), true);
	}

}