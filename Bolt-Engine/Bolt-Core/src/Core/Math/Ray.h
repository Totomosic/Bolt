#pragma once
#include "Core/Math/Vectors/Vector3.h"

namespace Bolt
{

	struct BLT_API Ray
	{
	public:
		Vector3f Origin;
		Vector3f Direction;

	public:
		Ray(const Vector3f& origin, const Vector3f& direction);
		Ray();

		blt::string ToString() const;
		friend std::ostream& operator<<(std::ostream& stream, const Ray& ray);
	};

}