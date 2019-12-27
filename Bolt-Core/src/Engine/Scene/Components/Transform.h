#pragma once
#include "Core/Math/Maths.h"
#include "Component.h"

namespace Bolt
{

	BLT_API enum class Space
	{
		World,
		Local
	};

	class BLT_API Transform : public Component<Transform>
	{
	private:
		Vector3f m_LocalPosition;
		Quaternion m_LocalOrientation;
		Vector3f m_LocalScale;

		const Transform* m_Parent;
		mutable std::vector<Transform*> m_Children;

		mutable Matrix4f m_TransformMatrix;
		mutable Matrix4f m_InverseTransformMatrix;
		mutable bool m_IsValid;
		mutable bool m_UpdateOnInvalidate;

	public:
		Transform(Vector3f position = Vector3f(0.0f), Quaternion orientation = Quaternion::Identity(), Vector3f scale = Vector3f(1.0f));
		Transform(const Transform& other);
		Transform& operator=(const Transform& other);
		Transform(Transform&& other) noexcept;
		Transform& operator=(Transform&& other) noexcept;
		~Transform();

		bool GetUpdateOnInvalidate() const;
		void SetUpdateOnInvalidate(bool update);

		bool HasParent() const;
		const Transform& GetParent() const;
		void SetParent(const Transform* transform);

		const Vector3f& LocalPosition() const;
		const Quaternion& LocalOrientation() const;
		const Vector3f& LocalScale() const;
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

		void SetLocalX(float x);
		void SetLocalY(float y);
		void SetLocalZ(float z);
		void SetLocalXY(float x, float y);
		void SetLocalXY(const Vector2f& xy);
		void SetLocalXZ(float x, float z);
		void SetLocalXZ(const Vector2f& xz);
		void SetLocalYZ(float y, float z);
		void SetLocalYZ(const Vector2f& yz);
		
		void SetLocalXScale(float x);
		void SetLocalYScale(float y);
		void SetLocalZScale(float z);

		void Translate(const Vector3f& translation);
		void Translate(float x, float y, float z);
		void Rotate(const Quaternion& rotation);
		void Rotate(float angle, Vector3f axis, Space rotateSpace = Space::World);

		void Reset();

	private:
		void RecalculateMatrix() const;
		void Invalidate() const;
		void CheckRecalculate() const;

	};

}