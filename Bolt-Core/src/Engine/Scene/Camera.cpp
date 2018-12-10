#include "Types.h"

#include "Camera.h"

namespace Bolt
{

	Plane TransformPlane(const Matrix4f& transform, const Plane& plane)
	{
		Vector4f o = Vector4f(plane.xyz().Normalize() * plane.w, -1);
		Vector4f n = Vector4f(plane.xyz().Normalize(), 0);
		o = transform * o;
		n = transform.Inverse().Transpose() * n;
		return Plane(n.xyz().Normalize(), o.xyz().Dot(n.xyz().Normalize()));
	}

	void Projection::GetPlanes(const Matrix4f& viewMatrix, Plane* outPlanes) const
	{
		Matrix4f transform = (ProjectionMatrix * viewMatrix).Inverse();
		Plane nearPlane = Plane(0.0f, 0.0f, 1.0, 1.0f);
		Plane farPlane = Plane(0.0f, 0.0f, -1.0f, 1.0f);
		Plane leftPlane = Plane(1.0f, 0.0f, 0.0f, 1.0f);
		Plane rightPlane = Plane(-1.0f, 0.0f, 0.0f, 1.0f);
		Plane bottomPlane = Plane(0.0f, 1.0f, 0.0f, 1.0f);
		Plane topPlane = Plane(0.0f, -1.0f, 0.0f, 1.0f);
		nearPlane = TransformPlane(transform, nearPlane);
		farPlane = TransformPlane(transform, farPlane);
		leftPlane = TransformPlane(transform, leftPlane);
		rightPlane = TransformPlane(transform, rightPlane);
		bottomPlane = TransformPlane(transform, bottomPlane);
		topPlane = TransformPlane(transform, topPlane);
		outPlanes[0] = nearPlane;
		outPlanes[1] = farPlane;
		outPlanes[2] = leftPlane;
		outPlanes[3] = rightPlane;
		outPlanes[4] = bottomPlane;
		outPlanes[5] = topPlane;
	}

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

	float Camera::ViewWidth() const
	{
		return CameraProjection().ViewFrustum.Width();
	}

	float Camera::ViewHeight() const
	{
		return CameraProjection().ViewFrustum.Height();
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