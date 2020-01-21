#include "bltpch.h"
#include "Camera.h"

namespace Bolt
{

	Camera::Camera() : Camera(Matrix4f::Identity())
	{
	}

	Camera::Camera(const Matrix4f& projection)
		: m_ProjectionMatrix(projection)
	{
	}

	const Matrix4f& Camera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	void Camera::SetProjectionMatrix(const Matrix4f& projection)
	{
		m_ProjectionMatrix = projection;
	}

}