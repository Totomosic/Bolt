#pragma once
#include "Component.h"
#include "Core/Math/Matrices/Matrix4.h"

namespace Bolt
{

	class BLT_API Camera : public Component<Camera>
	{
	private:
		Matrix4f m_ProjectionMatrix;

	public:
		Camera();
		Camera(const Matrix4f& projection);

		const Matrix4f& GetProjectionMatrix() const;
		void SetProjectionMatrix(const Matrix4f& projection);

	};

}