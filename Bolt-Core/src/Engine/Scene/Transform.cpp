#include "bltpch.h"

#include "Transform.h"

namespace Bolt
{

	Transform::Transform(Vector3f position, Quaternion orientation, Vector3f scale)
		: m_Parent(nullptr), m_Children(), m_LocalPosition(position), m_LocalOrientation(orientation), m_LocalScale(scale), m_TransformMatrix(Matrix4f::Identity()), m_InverseTransformMatrix(Matrix4f::Identity()), m_IsValid(false), m_UpdateOnInvalidate(false)
	{
		RecalculateMatrix();
	}

	Transform::Transform(Transform&& other)
		: m_Parent(other.m_Parent), m_Children(std::move(other.m_Children)), m_LocalPosition(other.m_LocalPosition), m_LocalOrientation(other.m_LocalOrientation), m_LocalScale(other.m_LocalScale),
		m_TransformMatrix(other.m_TransformMatrix), m_InverseTransformMatrix(other.m_InverseTransformMatrix), m_IsValid(other.m_IsValid), m_UpdateOnInvalidate(other.m_UpdateOnInvalidate)
	{
		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.push_back(this);
		}
		for (Transform* t : m_Children)
		{
			t->m_Parent = this;
		}
	}

	Transform& Transform::operator=(Transform&& other)
	{
		m_Parent = other.m_Parent;
		m_Children = std::move(other.m_Children);
		m_LocalPosition = other.m_LocalPosition;
		m_LocalOrientation = other.m_LocalOrientation;
		m_LocalScale = other.m_LocalScale;
		m_TransformMatrix = other.m_TransformMatrix;
		m_InverseTransformMatrix = other.m_InverseTransformMatrix;
		m_IsValid = other.m_IsValid;
		m_UpdateOnInvalidate = other.m_UpdateOnInvalidate;
		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.push_back(this);
		}
		for (Transform* t : m_Children)
		{
			t->m_Parent = this;
		}
		return *this;
	}

	Transform::~Transform()
	{
		if (m_Parent != nullptr)
		{
			auto it = std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this);
			if (it != m_Parent->m_Children.end())
			{
				m_Parent->m_Children.erase(it);
			}
		}
	}

	const Transform& Transform::Parent() const
	{
		return *m_Parent;
	}

	bool Transform::HasParent() const
	{
		return m_Parent != nullptr;
	}

	bool Transform::GetUpdateOnInvalidate() const
	{
		return m_UpdateOnInvalidate;
	}

	void Transform::SetParent(const Transform* transform)
	{
		if (transform == nullptr && m_Parent != nullptr)
		{
			auto it = std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this);
			if (it != m_Parent->m_Children.end())
			{
				m_Parent->m_Children.erase(it);
			}
		}
		else if (transform != nullptr)
		{
			transform->m_Children.push_back(this);
		}
		m_Parent = (Transform*)transform;
		RecalculateMatrix();
	}

	void Transform::SetUpdateOnInvalidate(bool update)
	{
		m_UpdateOnInvalidate = update;
	}

	int Transform::ChildCount() const
	{
		return m_Children.size();
	}

	const Vector3f& Transform::LocalPosition() const
	{
		return m_LocalPosition;
	}

	const Quaternion& Transform::LocalOrientation() const
	{
		return m_LocalOrientation;
	}

	const Vector3f& Transform::LocalScale() const
	{
		return m_LocalScale;
	}

	Vector3f Transform::Position() const
	{
		CheckRecalculate();
		return m_TransformMatrix.Column(3).xyz();
	}

	Quaternion Transform::Orientation() const
	{
		CheckRecalculate();
		if (HasParent())
		{
			return Parent().Orientation() * m_LocalOrientation;
		}
		return m_LocalOrientation;
	}

	Vector3f Transform::Scale() const
	{
		CheckRecalculate();
		if (HasParent())
		{
			return Parent().Scale() * m_LocalScale;
		}
		return m_LocalScale;
	}

	const Matrix4f& Transform::TransformMatrix() const
	{
		CheckRecalculate();
		return m_TransformMatrix;
	}

	const Matrix4f& Transform::InverseTransformMatrix() const
	{
		CheckRecalculate();
		return m_InverseTransformMatrix;
	}

	Vector3f Transform::Forward() const
	{
		return Orientation() * -Vector3f::Forward();
	}

	Vector3f Transform::Right() const
	{
		return Orientation() * Vector3f::Right();
	}

	Vector3f Transform::Up() const
	{
		return Orientation() * Vector3f::Up();
	}

	void Transform::SetLocalPosition(const Vector3f& position)
	{
		m_LocalPosition = position;
		Invalidate();
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{
		SetLocalPosition(Vector3f(x, y, z));
	}

	void Transform::SetLocalOrientation(const Quaternion& orientation)
	{
		m_LocalOrientation = orientation;
		Invalidate();
	}

	void Transform::SetLocalScale(const Vector3f& scale)
	{
		m_LocalScale = scale;
		Invalidate();
	}

	void Transform::SetLocalScale(float x, float y, float z)
	{
		SetLocalScale(Vector3f(x, y, z));
	}

	void Transform::SetLocalX(float x)
	{
		SetLocalPosition(x, m_LocalPosition.y, m_LocalPosition.z);
	}

	void Transform::SetLocalY(float y)
	{
		SetLocalPosition(m_LocalPosition.x, y, m_LocalPosition.z);
	}

	void Transform::SetLocalZ(float z)
	{
		SetLocalPosition(m_LocalPosition.x, m_LocalPosition.y, z);
	}

	void Transform::SetLocalXY(float x, float y)
	{
		SetLocalPosition(x, y, m_LocalPosition.z);
	}

	void Transform::SetLocalXY(const Vector2f& xy)
	{
		SetLocalPosition(xy.x, xy.y, m_LocalPosition.z);
	}

	void Transform::SetLocalXZ(float x, float z)
	{
		SetLocalPosition(x, m_LocalPosition.y, z);
	}

	void Transform::SetLocalXZ(const Vector2f& xz)
	{
		SetLocalPosition(xz.x, m_LocalPosition.y, xz.y);
	}

	void Transform::SetLocalYZ(float y, float z)
	{
		SetLocalPosition(m_LocalPosition.x, y, z);
	}

	void Transform::SetLocalYZ(const Vector2f& yz)
	{
		SetLocalPosition(m_LocalPosition.x, yz.x, yz.y);
	}

	void Transform::SetLocalXScale(float x)
	{
		SetLocalScale(x, m_LocalScale.y, m_LocalScale.z);
	}

	void Transform::SetLocalYScale(float y)
	{
		SetLocalScale(m_LocalScale.x, y, m_LocalScale.z);	
	}

	void Transform::SetLocalZScale(float z)
	{
		SetLocalScale(m_LocalScale.x, m_LocalScale.y, z);
	}

	void Transform::Translate(const Vector3f& translation)
	{
		SetLocalPosition(m_LocalPosition + translation);
	}

	void Transform::Translate(float x, float y, float z)
	{
		Translate(Vector3f(x, y, z));
	}

	void Transform::Rotate(const Quaternion& rotation)
	{
		SetLocalOrientation(m_LocalOrientation * rotation);
	}

	void Transform::Rotate(float angle, Vector3f axis, Space rotationSpace)
	{
		if (rotationSpace == Space::World)
		{
			axis = Orientation().Inverse() * axis;
		}
		Rotate(Quaternion::FromAngleAxis(angle, axis));
	}

	void Transform::Reset()
	{
		m_LocalPosition = Vector3f(0, 0, 0);
		m_LocalOrientation = Quaternion::Identity();
		m_LocalScale = Vector3f(1, 1, 1);
		Invalidate();
	}

	void Transform::Transfer(XMLserializer& backend, bool isWriting)
	{
		BLT_TRANSFER(backend, m_LocalPosition);
		BLT_TRANSFER(backend, m_LocalOrientation);
		BLT_TRANSFER(backend, m_LocalScale);

		BLT_TRANSFER(backend, m_Parent);
		BLT_TRANSFER(backend, m_Children);
	}

	void Transform::RecalculateMatrix() const
	{
		m_TransformMatrix = Matrix4f::Translation(m_LocalPosition) * m_LocalOrientation.ToMatrix4f() * Matrix4f::Scale(m_LocalScale);
		if (HasParent())
		{
			m_TransformMatrix = Parent().TransformMatrix() * m_TransformMatrix;
		}
		m_InverseTransformMatrix = m_TransformMatrix.Inverse();
		m_IsValid = true;
	}

	void Transform::Invalidate() const
	{
		m_IsValid = false;
		if (m_UpdateOnInvalidate)
		{
			RecalculateMatrix();
		}
		for (const Transform* child : m_Children)
		{
			child->Invalidate();
		}
	}

	void Transform::CheckRecalculate() const
	{
		if (!m_IsValid)
		{
			RecalculateMatrix();
		}
	}

}