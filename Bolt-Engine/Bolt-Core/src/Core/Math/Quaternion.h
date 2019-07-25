#pragma once
#include "Matrices/Matrix4.h"

namespace Bolt
{

	struct BLT_API Quaternion
	{
	public:
		float x;
		float y;
		float z; 
		float w;

	public:
		Quaternion();
		Quaternion(const Vector4f& values);
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Quaternion& other);

		Matrix4f ToMatrix4f() const;
		Vector4f ToAngleAxis() const;
		float GetLength() const;
		float GetLengthSqrd() const;
		float Dot(const Quaternion& q) const;
		Quaternion Conjugate() const;
		float Norm() const;
		Quaternion Inverse() const;
		Quaternion Normalize() const;

		float& operator[](int index);

		friend Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
		friend Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
		friend Quaternion operator-(const Quaternion& q1);
		friend Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
		friend Quaternion operator*(const Quaternion& q1, float s);
		friend Quaternion operator*(float s, const Quaternion& q1);
		friend Vector3f operator*(const Quaternion& q1, const Vector3f& v);
		friend Quaternion operator/(const Quaternion& q1, const Quaternion& q2);
		friend Quaternion operator/(const Quaternion& q1, float s);

		Quaternion& operator+=(const Quaternion& q2);
		Quaternion& operator-=(const Quaternion& q2);
		Quaternion& operator*=(const Quaternion& q2);
		Quaternion& operator*=(float s);
		Quaternion& operator/=(const Quaternion& q2);
		Quaternion& operator/=(float s);

		friend bool operator==(const Quaternion& q1, const Quaternion& q2);
		friend bool operator!=(const Quaternion& q1, const Quaternion& q2);

		template<typename Other, typename OtherBase>
		void AddVector3(const Vector3<Other, OtherBase>& vec)
		{
			Quaternion q(vec.x, vec.y, vec.z, 0.0f);
			q *= *this;
			w += q.w * 0.5f;
			x += q.x * 0.5f;
			y += q.y * 0.5f;
			z += q.z * 0.5f;
		}

		blt::string ToString() const;

		void Transfer(XMLserializer& backend, bool isWriting);

	public:
		static Quaternion Log(const Quaternion& q);
		static Quaternion Exp(const Quaternion& q);
		static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
		static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
		static Quaternion Squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& ta, const Quaternion& tb, float t);
		static Quaternion SimpleSquad(const Quaternion& prev, Quaternion q1, Quaternion q2, Quaternion post, float t);
		static Quaternion Spline(const Quaternion& pre, const Quaternion& q, const Quaternion& post);

		static Quaternion Zero();
		static Quaternion Identity();
		static Quaternion FromAngleAxis(float angle, const Vector3f& axis);
		static Quaternion FromEuler(float pitch, float yaw, float roll);
		static Quaternion FromRotationMat(const Matrix4f& rotation);
		static Quaternion LookAt(const Vector3f& forward, const Vector3f& up = Vector3f::Up());

		friend std::ostream& operator<<(std::ostream& stream, const Quaternion& q);

	};

}