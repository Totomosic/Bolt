#pragma once
#include "Vector2.h"

namespace Bolt
{

	template<typename T, typename Base = T>
	struct BLT_API Vector3
	{
	public:
		T x;
		T y;
		T z;

	public:
		Vector3(T x, T y, T z)
			: x(x), y(y), z(z)
		{

		}

		template<typename Other, typename OtherBase>
		Vector3(const Vector2<Other, OtherBase>& xy, T z) : Vector3(xy.x, xy.y, z)
		{

		}

		template<typename Other, typename OtherBase>
		Vector3(const Vector2<Other, OtherBase>& xy) : Vector3(xy.x, xy.y, (T)0)
		{

		}

		Vector3(T value) : Vector3(value, value, value)
		{

		}

		Vector3() : Vector3((T)0)
		{

		}

		template<typename Other, typename OtherBase>
		Vector3(const Vector3<Other, OtherBase>& other) : Vector3(other.x, other.y, other.z)
		{

		}

		template<typename Other, typename OtherBase>
		Vector3<T, Base>& operator=(const Vector3<Other, OtherBase>& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		void Clear()
		{
			x = (Base)0.0;
			y = (Base)0.0;
			z = (Base)0.0;
		}

		const T Get(int index) const
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				BLT_ASSERT(false, "Index out of range");
			}
		}

		T& Get(int index)
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				BLT_ASSERT(false, "Index out of range");
			}
		}

		const T operator[](int index) const
		{
			return Get(index);
		}

		T& operator[](int index)
		{
			return Get(index);
		}

		Base LengthSqrd() const
		{
			return x * x + y * y + z * z;
		}

		Base Length() const
		{
			return sqrt(LengthSqrd());
		}

		Base MagnitudeSqrd() const
		{
			return LengthSqrd();
		}

		Base Magnitude() const
		{
			return Length();
		}

		template<typename Other, typename OtherBase>
		Base Dot(const Vector3<Other, OtherBase>& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		template<typename Other, typename OtherBase>
		Vector3<Base> Cross(const Vector3<Other, OtherBase>& other) const
		{
			return Vector3<Base>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		Vector3<Base> Normalize() const
		{
			return *this / Length();
		}

		template<typename Other, typename OtherBase>
		Vector3<Base> Add(const Vector3<Other, OtherBase>& other) const
		{
			return Vector3<Base>(x + other.x, y + other.y, z + other.z);
		}

		Vector3<Base> Add(Base other) const
		{
			return Vector3<Base>(x + other, y + other, z + other);
		}

		template<typename Other, typename OtherBase>
		Vector3<Base> Sub(const Vector3<Other, OtherBase>& other) const
		{
			return Vector3<Base>(x - other.x, y - other.y, z - other.z);
		}

		Vector3<Base> Sub(Base other) const
		{
			return Vector3<Base>(x - other, y - other, z - other);
		}

		template<typename Other, typename OtherBase>
		Vector3<Base> Mul(const Vector3<Other, OtherBase>& other) const
		{
			return Vector3<Base>(x * other.x, y * other.y, z * other.z);
		}

		Vector3<Base> Mul(Base other) const
		{
			return Vector3<Base>(x * other, y * other, z * other);
		}

		template<typename Other, typename OtherBase>
		Vector3<Base> Div(const Vector3<Other, OtherBase>& other) const
		{
			return Vector3<Base>(x / other.x, y / other.y, z / other.z);
		}

		Vector3<Base> Div(Base other) const
		{
			return Vector3<Base>(x / other, y / other, z / other);
		}

		template<typename Other, typename OtherBase>
		friend Vector3<Base> operator+(const Vector3<T, Base>& left, const Vector3<Other, OtherBase>& right)
		{
			return left.Add(right);
		}

		friend Vector3<Base> operator+(const Vector3<T, Base>& left, Base right)
		{
			return left.Add(right);
		}

		template<typename Other, typename OtherBase>
		friend Vector3<Base> operator+(T left, const Vector3<Other, OtherBase>& right)
		{
			return Vector3<Base>(left + right.x, left + right.y, left + right.z);
		}

		template<typename Other, typename OtherBase>
		friend Vector3<Base> operator-(const Vector3<T, Base>& left, const Vector3<Other, OtherBase>& right)
		{
			return left.Sub(right);
		}

		friend Vector3<Base> operator-(const Vector3<T, Base>& left, Base right)
		{
			return left.Sub(right);
		}

		template<typename Other, typename OtherBase>
		friend Vector3<Base> operator-(T left, const Vector3<Other, OtherBase>& right)
		{
			return Vector3<Base>(left - right.x, left - right.y, left - right.z);
		}

		template<typename Other, typename OtherBase>
		friend Vector3<Base> operator*(const Vector3<T, Base>& left, const Vector3<Other, OtherBase>& right)
		{
			return left.Mul(right);
		}

		friend Vector3<Base> operator*(const Vector3<T, Base>& left, Base right)
		{
			return left.Mul(right);
		}

		template<typename Other, typename OtherBase>
		friend Vector3<Base> operator*(T left, const Vector3<Other, OtherBase>& right)
		{
			return Vector3<Base>(left * right.x, left * right.y, left * right.z);
		}

		template<typename Other, typename OtherBase>
		friend Vector3<Base> operator/(const Vector3<T, Base>& left, const Vector3<Other, OtherBase>& right)
		{
			return left.Div(right);
		}

		friend Vector3<Base> operator/(const Vector3<T, Base>& left, Base right)
		{
			return left.Div(right);
		}

		template<typename Other, typename OtherBase>
		friend Vector3<Base> operator/(T left, const Vector3<Other, OtherBase>& right)
		{
			return Vector3<Base>(left / right.x, left / right.y, left / right.z);
		}

		template<typename Other, typename OtherBase>
		Vector3<T, Base>& operator+=(const Vector3<Other, OtherBase>& other)
		{
			*this = *this + other;
			return *this;
		}

		Vector3<T, Base>& operator+=(Base other)
		{
			*this = *this + other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Vector3<T, Base>& operator-=(const Vector3<Other, OtherBase>& other)
		{
			*this = *this - other;
			return *this;
		}

		Vector3<T, Base>& operator-=(Base other)
		{
			*this = *this - other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Vector3<T, Base>& operator*=(const Vector3<Other, OtherBase>& other)
		{
			*this = *this * other;
			return *this;
		}

		Vector3<T, Base>& operator*=(Base other)
		{
			*this = *this * other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Vector3<T, Base>& operator/=(const Vector3<Other, OtherBase>& other)
		{
			*this = *this / other;
			return *this;
		}

		Vector3<T, Base>& operator/=(Base other)
		{
			*this = *this / other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		bool operator==(const Vector3<Other, OtherBase>& other) const
		{
			return (x == other.x && y == other.y && z == other.z);
		}

		template<typename Other, typename OtherBase>
		bool operator!=(const Vector3<Other, OtherBase>& other) const
		{
			return !(*this == other);
		}

		Vector3<Base> operator-() const
		{
			return Vector3<T, Base>(-x, -y, -z);
		}

		blt::string ToString() const
		{
			return "Vector3(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
		}

		friend std::ostream& operator<<(std::ostream& stream, const Vector3<T, Base>& vector)
		{
			stream << vector.ToString();
			return stream;
		}

		void Transfer(XMLserializer& backend, bool isWriting)
		{
			BLT_TRANSFER(backend, x);
			BLT_TRANSFER(backend, y);
			BLT_TRANSFER(backend, z);
		}

		Vector2<Base> xy() const
		{
			return Vector2<Base>(x, y);
		}

		Vector2<T&, Base> xy()
		{
			return Vector2<T&, Base>(x, y);
		}

		Vector2<Base> xz() const
		{
			return Vector2<Base>(x, z);
		}

		Vector2<T&, Base> xz()
		{
			return Vector2<T&, Base>(x, z);
		}

		template<typename Other, typename OtherBase>
		static Vector3<Base> Cross(const Vector3<T, Base>& left, const Vector3<Other, OtherBase>& right)
		{
			return left.Cross(right);
		}

		template<typename Other, typename OtherBase>
		static T Distance(const Vector3<T, Base>& left, const Vector3<Other, OtherBase>& right)
		{
			return (left - right).Length();
		}

		static void MakeOrthonormalBasis(Vector3<T, Base>* a, Vector3<T, Base>* b, Vector3<T, Base>* c)
		{
			*(a) = *(a)->Normalize();
			(*c) = Cross(*a, *b);
			if (c->MagnitudeSqrd() == 0.0f)
			{
				return;
			}
			(*c) = (*c).Normalize();
			(*b) = Cross(*c, *a);
		}
						
		static void OrthoNormalize(Vector3<T, Base>* normal, Vector3<T, Base>* tangent)
		{	
			*normal = normal->Normalize();	
			Vector3<T, Base> v = Cross(*normal, *tangent);	
			v = v.Normalize();	
			*tangent = Cross(v, *normal);	
		}	

		static Vector3<T, Base> SmoothDamp(const Vector3<T, Base>& current, const Vector3<T, Base>& end, Vector3<T, Base>* currentVelocity, T smoothTime, T deltaTime, T maxSpeed = std::numeric_limits<T>::infinity())
		{
			smoothTime = max((T)0.0001, smoothTime);
			float num = 2.0f / smoothTime;
			float num2 = num * deltaTime;
			float d = 1.0f / (1.0f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
			Vector3<T, Base> vector = current - end;
			Vector3<T, Base> vector2 = end;
			float maxLength = maxSpeed * smoothTime;
			if (vector.Length() > maxSpeed)
			{
				vector = vector.Normalize() * maxSpeed;
			}
			Vector3<T, Base> target = current - vector;
			Vector3<T, Base> vector3 = (*currentVelocity + num * vector) * deltaTime;
			*currentVelocity = (*currentVelocity - num * vector3) * d;
			Vector3<T, Base> vector4 = target + (vector + vector3) * d;
			if ((vector2 - current).Dot(vector4 - vector2) > 0.0f)
			{
				vector4 = vector2;
				*currentVelocity = (vector4 - vector2) / deltaTime;
			}
			return vector4;
		}
						
		static Vector3<Base> Right()
		{	
			return Vector3<Base>((Base)1, (Base)0, (Base)0);
		}	
						
		static Vector3<Base> Up()
		{	
			return Vector3<Base>((Base)0, (Base)1, (Base)0);
		}	
						
		static Vector3<Base> Forward()
		{	
			return Vector3<Base>((Base)0, (Base)0, (Base)1);
		}

	};

	typedef Vector3<float> Vector3f;
	typedef Vector3<int> Vector3i;
	typedef Vector3<double> Vector3d;

	typedef Vector3<float&, float> Vector3fr;
	typedef Vector3<int&, int> Vector3ir;
	typedef Vector3<double&, double> Vector3dr;

}