#include "bltpch.h"

#include "Ray.h"

namespace Bolt
{

	Ray::Ray(const Vector3f& origin, const Vector3f& direction)
		: Origin(origin), Direction(direction.Normalize())
	{

	}

	Ray::Ray() : Ray(Vector3f(0.0f), Vector3f::Forward())
	{
		
	}

	blt::string Ray::ToString() const
	{
		return "Ray(" + Origin.ToString() + ", " + Direction.ToString() + ")";
	}

	std::ostream& operator<<(std::ostream& stream, const Ray& ray)
	{
		stream << ray.ToString();
		return stream;
	}
}