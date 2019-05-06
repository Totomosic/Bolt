#pragma once
#include "GameObject.h"
#include "BasicModels.h"

#include "Components/MeshRenderer.h"

namespace Bolt
{

	struct Layer;

	class BLT_API ObjectFactory
	{
	private:
		Layer* m_CurrentLayer;
		GameObject* m_CurrentParent;
		std::vector<ObjectPrefab> m_Prefabs;

	public:
		ObjectFactory();
		ObjectFactory(Layer& layer);
		ObjectFactory(GameObject* parent);
		ObjectFactory(const ObjectFactory& other) = delete;
		ObjectFactory& operator=(const ObjectFactory& other) = delete;
		ObjectFactory(ObjectFactory&& other) = default;
		ObjectFactory& operator=(ObjectFactory&& other) = default;
		~ObjectFactory() = default;

		Layer* CurrentLayer() const;
		GameObject* CurrentParent() const;
		void SetCurrentLayer(Layer& layer);
		void SetCurrentParent(GameObject* parent);

		id_t AddPrefab(ObjectPrefab&& prefab);
		const ObjectPrefab* GetPrefab(id_t index) const;
		ObjectPrefab* GetPrefab(id_t index);
		void ClearPrefabs();

		GameObject* Instantiate(Transform transform = Transform()) const;
		GameObject* Instantiate(float x, float y, float z, const Quaternion& orientation = Quaternion::Identity(), const Vector3f& scale = Vector3f(1, 1, 1)) const;
		GameObject* Instantiate(const ObjectPrefab* prefab) const;
		GameObject* Instantiate(const ObjectPrefab* prefab, Transform transform) const;
		GameObject* Instantiate(const ObjectPrefab* prefab, float x, float y, float z, const Quaternion& orientation = Quaternion::Identity(), const Vector3f& scale = Vector3f(1, 1, 1)) const;
		GameObject* Instantiate(std::unique_ptr<MeshRenderer>&& meshRenderer, Transform transform = Transform()) const;
		GameObject* Instantiate(Mesh mesh, Transform transform = Transform()) const;

		GameObject* Rectangle(float width, float height, const Color& color = Color::White, Transform transform = Transform()) const;
		GameObject* Rectangle(float width, float height, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;
		GameObject* Image(float width, float height, const ResourcePtr<const Texture2D>& image, Transform transform = Transform()) const;
		GameObject* Ellipse(float width, float height, const Color& color = Color::White, Transform transform = Transform()) const;
		GameObject* Ellipse(float width, float height, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;

		GameObject* Cuboid(float width, float height, float depth, const Color& color = Color::White, Transform transform = Transform()) const;
		GameObject* Cuboid(float width, float height, float depth, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;
		GameObject* Grid(float width, float depth, int xVertices = 2, int zVertices = 2, const Color& color = Color::White, Transform transform = Transform()) const;
		GameObject* Grid(float width, float depth, int xVertices, int zVertices, std::unique_ptr<Material>&& material, Transform transform = Transform()) const;
		GameObject* Line(const Vector3f& p0, const Vector3f& p1, const Color& color = Color::White) const;

	};

}