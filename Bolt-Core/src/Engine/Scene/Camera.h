#pragma once
#include "GameObject.h"

namespace Bolt
{

	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};

	struct BLT_API CameraView
	{
	public:
		Matrix4f ViewMatrix;
		Matrix4f ProjectionMatrix;
	};

	struct BLT_API Projection
	{
	public:
		Frustum ViewFrustum;
		ProjectionType Type;
		Matrix4f ProjectionMatrix;

	public:
		static Projection Perspective(float fovy, float aspect, float nearPlane, float farPlane);
		static Projection Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

	};

	class BLT_API Camera : public GameObject
	{
	private:
		Projection m_Projection;

	public:
		Camera();
		Camera(const Frustum& viewFrustum, ProjectionType type);
		Camera(const Projection& projection);

		const Projection& CameraProjection() const;
		const Matrix4f& ProjectionMatrix() const;
		const Matrix4f& ViewMatrix() const;

		void SetViewFrustum(const Frustum& frustum);
		void SetProjectionType(ProjectionType type);
		void SetProjection(const Projection& projection);

		Ray NDCToWorldRay(const Vector3f& ndcCoordinate) const;

		friend struct Projection;
		friend struct Scene;

	private:
		static void CalculateProjectionMatrix(Projection& projection);

	};

}