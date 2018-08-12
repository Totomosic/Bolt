#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	enum class Space
	{
		World,
		Local
	};

	struct BLT_API Transform
	{
	private:
		const Transform* m_Parent;
		mutable std::vector<Transform*> m_Children;

		Vector3f m_Position;
		Quaternion m_Orientation;
		Vector3f m_Scale;

		mutable Matrix4f m_TransformMatrix;
		mutable Matrix4f m_InverseTransformMatrix;
		mutable bool m_IsValid;

	public:
		Transform(Vector3f position = Vector3f(0.0f, 0.0f, 0.0f), Quaternion orientation = Quaternion::Identity(), Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f));
		Transform(const Transform& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform(Transform&& other);
		Transform& operator=(Transform&& other);
		~Transform();

		const Transform& Parent() const;
		bool HasParent() const;
		void SetParent(const Transform* transform);

		Vector3f Position() const;
		Quaternion Orientation() const;
		Vector3f Scale() const;
		const Matrix4f& TransformMatrix() const;
		const Matrix4f& InverseTransformMatrix() const;
		Vector3f Forward() const;
		Vector3f Right() const;
		Vector3f Up() const;

		void SetLocalPosition(const Vector3f& position);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalOrientation(const Quaternion& orientation);
		void SetLocalScale(const Vector3f& scale);
		void SetLocalScale(float x, float y, float z);

		void Translate(const Vector3f& translation);
		void Translate(float x, float y, float z);
		void Rotate(const Quaternion& rotation);
		void Rotate(float angle, Vector3f axis, Space rotateSpace = Space::World);

	private:
		void RecalculateMatrix() const;
		void Invalidate() const;
		void CheckRecalculate() const;

	};

}