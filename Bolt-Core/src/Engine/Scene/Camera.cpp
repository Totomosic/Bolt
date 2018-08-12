#include "Camera.h"

namespace Bolt
{

	Projection Projection::Perspective(float fovy, float aspect, float nearPlane, float farPlane)
	{
		return { Frustum::Perspective(fovy, aspect, nearPlane, farPlane), ProjectionType::Perspective, Matrix4f::Perspective(fovy, aspect, nearPlane, farPlane) };
	}

	Projection Projection::Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	{
		return { { left, right, bottom, top, nearPlane, farPlane }, ProjectionType::Orthographic, Matrix4f::Orthographic(left, right, bottom, top, nearPlane, farPlane) };
	}

	Camera::Camera() : Camera({ -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f }, ProjectionType::Orthographic)
	{
		
	}

	Camera::Camera(const Frustum& viewFrustum, ProjectionType type) : Camera({ viewFrustum, type, Matrix4f::Identity() })
	{
		
	}

	Camera::Camera(const Projection& projection) : GameObject(),
		m_Projection(projection)
	{
		CalculateProjectionMatrix(m_Projection);
	}

	const Projection& Camera::CameraProjection() const
	{
		return m_Projection;
	}

	const Matrix4f& Camera::ProjectionMatrix() const
	{
		return m_Projection.ProjectionMatrix;
	}

	const Matrix4f& Camera::ViewMatrix() const
	{
		return m_Transform.InverseTransformMatrix();
	}

	void Camera::SetViewFrustum(const Frustum& frustum)
	{
		m_Projection.ViewFrustum = frustum;
		CalculateProjectionMatrix(m_Projection);
	}

	void Camera::SetProjectionType(ProjectionType type)
	{
		m_Projection.Type = type;
		CalculateProjectionMatrix(m_Projection);
	}

	void Camera::SetProjection(const Projection& projection)
	{
		m_Projection = projection;
		CalculateProjectionMatrix(m_Projection);
	}

	Ray Camera::NDCToWorldRay(const Vector3f& ndcCoordinate) const
	{
		if (m_Projection.Type == ProjectionType::Orthographic)
		{
			return Ray(m_Transform.Position() + ndcCoordinate, -Vector3f::Forward());
		}

		Matrix4f vp = ProjectionMatrix() * m_Transform.InverseTransformMatrix();
		Matrix4f ivp = vp.Inverse();

		Vector4f nearNDC = Vector4f(ndcCoordinate.xy(), -1.0f, 1.0f);
		Vector4f farNDC = Vector4f(ndcCoordinate.xy(), 1.0f, 1.0f);

		Vector4f nearPoint = ivp * nearNDC;
		Vector4f farPoint = ivp * farNDC;

		nearPoint.x /= nearPoint.w;
		nearPoint.y /= nearPoint.w;
		nearPoint.z /= nearPoint.w;

		farPoint.x /= farPoint.w;
		farPoint.y /= farPoint.w;
		farPoint.z /= farPoint.w;

		Vector3f dir = (farPoint.xyz() - nearPoint.xyz()).Normalize();
		return Ray(m_Transform.Position(), dir);
	}

	void Camera::CalculateProjectionMatrix(Projection& projection)
	{
		if (projection.Type == ProjectionType::Orthographic)
		{
			projection.ProjectionMatrix = Matrix4f::Orthographic(projection.ViewFrustum);
		}
		else if (projection.Type == ProjectionType::Perspective)
		{
			projection.ProjectionMatrix = Matrix4f::Perspective(projection.ViewFrustum);
		}
	}

}