#include "Types.h"

#include "Transform.h"

namespace Bolt
{

	Transform::Transform(Vector3f position, Quaternion orientation, Vector3f scale)
		: m_Parent(nullptr), m_Children(), m_Position(position), m_Orientation(orientation), m_Scale(scale), m_TransformMatrix(Matrix4f::Identity()), m_InverseTransformMatrix(Matrix4f::Identity()), m_IsValid(false), m_UpdateOnInvalidate(false)
	{
		RecalculateMatrix();
	}

	Transform::Transform(Transform&& other)
		: m_Parent(other.m_Parent), m_Children(std::move(other.m_Children)), m_Position(other.m_Position), m_Orientation(other.m_Orientation), m_Scale(other.m_Scale), 
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
		m_Position = other.m_Position;
		m_Orientation = other.m_Orientation;
		m_Scale = other.m_Scale;
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
			return Parent().Orientation() * m_Orientation;
		}
		return m_Orientation;
	}

	Vector3f Transform::Scale() const
	{
		CheckRecalculate();
		if (HasParent())
		{
			return Parent().Scale() * m_Scale;
		}
		return m_Scale;
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
		m_Position = position;
		Invalidate();
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{
		SetLocalPosition(Vector3f(x, y, z));
	}

	void Transform::SetLocalOrientation(const Quaternion& orientation)
	{
		m_Orientation = orientation;
		Invalidate();
	}

	void Transform::SetLocalScale(const Vector3f& scale)
	{
		m_Scale = scale;
		Invalidate();
	}

	void Transform::SetLocalScale(float x, float y, float z)
	{
		SetLocalScale(Vector3f(x, y, z));
	}

	void Transform::Translate(const Vector3f& translation)
	{
		SetLocalPosition(m_Position + translation);
	}

	void Transform::Translate(float x, float y, float z)
	{
		Translate(Vector3f(x, y, z));
	}

	void Transform::Rotate(const Quaternion& rotation)
	{
		SetLocalOrientation(m_Orientation * rotation);
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
		m_Position = Vector3f(0, 0, 0);
		m_Orientation = Quaternion::Identity();
		m_Scale = Vector3f(1, 1, 1);
		Invalidate();
	}

	void Transform::Transfer(XMLserializer& backend, bool isWriting)
	{
		BLT_TRANSFER(backend, m_Position);
		BLT_TRANSFER(backend, m_Orientation);
		BLT_TRANSFER(backend, m_Scale);

		BLT_TRANSFER(backend, m_Parent);
		BLT_TRANSFER(backend, m_Children);
	}

	void Transform::RecalculateMatrix() const
	{
		m_TransformMatrix = Matrix4f::Translation(m_Position) * m_Orientation.ToMatrix4f() * Matrix4f::Scale(m_Scale);
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