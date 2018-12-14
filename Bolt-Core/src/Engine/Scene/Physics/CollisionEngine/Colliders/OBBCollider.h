#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	struct BLT_API OBBCollider
	{
	public:
		Cuboid Bounds;
		Quaternion Orientation;

	public:
		// Bottom Left Back
		inline Vector3f BLB() const { return Orientation * Vector3f(Bounds.Min.x, Bounds.Min.y, Bounds.Min.z); }
		// Bottom Left Front
		inline Vector3f BLF() const { return Orientation * Vector3f(Bounds.Min.x, Bounds.Min.y, Bounds.Max.z); }
		// Bottom Right Front
		inline Vector3f BRF() const { return Orientation * Vector3f(Bounds.Max.x, Bounds.Min.y, Bounds.Max.z); }
		// Bottom Right Back
		inline Vector3f BRB() const { return Orientation * Vector3f(Bounds.Max.x, Bounds.Min.y, Bounds.Min.z); }
		// Top Left Back
		inline Vector3f TLB() const { return Orientation * Vector3f(Bounds.Min.x, Bounds.Max.y, Bounds.Min.z); }
		// Top Left Front
		inline Vector3f TLF() const { return Orientation * Vector3f(Bounds.Min.x, Bounds.Max.y, Bounds.Max.z); }
		// Top Right Front
		inline Vector3f TRF() const { return Orientation * Vector3f(Bounds.Max.x, Bounds.Max.y, Bounds.Max.z); }
		// Top Right Back
		inline Vector3f TRB() const { return Orientation * Vector3f(Bounds.Max.x, Bounds.Max.y, Bounds.Min.z); }

		inline Vector3f Min() const { return BLB(); }
		inline Vector3f Max() const { return TRF(); }

	};

}