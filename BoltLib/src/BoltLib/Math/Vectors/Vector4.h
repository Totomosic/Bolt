#pragma once
#include "Vector3.h"

namespace Bolt
{

	template<typename T, typename Base = T>
	struct BLT_API Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

	public:
		constexpr Vector4(T x, T y, T z, T w)
			: x(x), y(y), z(z), w(w)
		{

		}

		template<typename Other, typename OtherBase>
		constexpr Vector4(const Vector3<Other, OtherBase>& xyz, T w) : Vector4(xyz.x, xyz.y, xyz.z, w)
		{

		}

		template<typename Other, typename OtherBase>
		constexpr Vector4(const Vector3<Other, OtherBase>& xyz) : Vector4(xyz.x, xyz.y, xyz.z, (T)1)
		{

		}

		template<typename Other, typename OtherBase>
		constexpr Vector4(const Vector2<Other, OtherBase>& xy, T z, T w) : Vector4(xy.x, xy.y, z, w)
		{

		}

		template<typename Other, typename OtherBase>
		constexpr Vector4(const Vector2<Other, OtherBase>& xy, T z) : Vector4(xy, z, (T)1)
		{

		}

		template<typename Other, typename OtherBase>
		constexpr Vector4(const Vector2<Other, OtherBase>& xy) : Vector4(xy, (T)0)
		{

		}

		constexpr Vector4(T xyz, T w) : Vector4(xyz, xyz, xyz, w)
		{

		}

		constexpr Vector4(T value) : Vector4(value, value)
		{

		}

		constexpr Vector4() : Vector4((T)0)
		{

		}

		template<typename Other, typename OtherBase>
		constexpr Vector4(const Vector4<Other, OtherBase>& other) : Vector4(other.x, other.y, other.z, other.w)
		{

		}

		template<typename Other, typename OtherBase>
		constexpr Vector4<T, Base>& operator=(const Vector4<Other, OtherBase>& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		void Clear()
		{
			x = (Base)0.0;
			y = (Base)0.0;
			z = (Base)0.0;
			w = (Base)0.0;
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
			case 3:
				return w;
			default:
				BLT_ASSERT(false, "Index out of range");
				return x;
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
			case 3:
				return w;
			default:
				BLT_ASSERT(false, "Index out of range");
				return x;
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
			return x * x + y * y + z * z + w * w;
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
		Base Dot(const Vector4<Other, OtherBase>& other) const
		{
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		Vector4<Base> Normalize() const
		{
			return *this / Length();
		}

		template<typename Other, typename OtherBase>
		Vector4<Base> Add(const Vector4<Other, OtherBase>& other) const
		{
			return Vector4<Base>(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		Vector4<Base> Add(Base other) const
		{
			return Vector4<Base>(x + other, y + other, z + other, w + other);
		}

		template<typename Other, typename OtherBase>
		Vector4<Base> Sub(const Vector4<Other, OtherBase>& other) const
		{
			return Vector4<Base>(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		Vector4<Base> Sub(Base other) const
		{
			return Vector4<Base>(x - other, y - other, z - other, w - other);
		}

		template<typename Other, typename OtherBase>
		Vector4<Base> Mul(const Vector4<Other, OtherBase>& other) const
		{
			return Vector4<Base>(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		Vector4<Base> Mul(Base other) const
		{
			return Vector4<Base>(x * other, y * other, z * other, w * other);
		}

		template<typename Other, typename OtherBase>
		Vector4<Base> Div(const Vector4<Other, OtherBase>& other) const
		{
			return Vector4<Base>(x / other.x, y / other.y, z / other.z, w / other.w);
		}

		Vector4<Base> Div(Base other) const
		{
			return Vector4<Base>(x / other, y / other, z / other, w / other);
		}

		template<typename Other, typename OtherBase>
		friend Vector4<Base> operator+(const Vector4<T>& left, const Vector4<Other, OtherBase>& right)
		{
			return left.Add(right);
		}

		friend Vector4<Base> operator+(const Vector4<T>& left, Base right)
		{
			return left.Add(right);
		}

		template<typename Other, typename OtherBase>
		friend Vector4<Base> operator+(T left, const Vector4<Other, OtherBase>& right)
		{
			return Vector4<T>(left + right.x, left + right.y, left + right.z, left + right.w);
		}

		template<typename Other, typename OtherBase>
		friend Vector4<Base> operator-(const Vector4<T>& left, const Vector4<Other, OtherBase>& right)
		{
			return left.Sub(right);
		}

		friend Vector4<Base> operator-(const Vector4<T>& left, Base right)
		{
			return left.Sub(right);
		}

		template<typename Other, typename OtherBase>
		friend Vector4<Base> operator-(T left, const Vector4<Other, OtherBase>& right)
		{
			return Vector4<Base>(left - right.x, left - right.y, left - right.z, left - right.w);
		}

		template<typename Other, typename OtherBase>
		friend Vector4<Base> operator*(const Vector4<T>& left, const Vector4<Other, OtherBase>& right)
		{
			return left.Mul(right);
		}

		friend Vector4<Base> operator*(const Vector4<T>& left, Base right)
		{
			return left.Mul(right);
		}

		template<typename Other, typename OtherBase>
		friend Vector4<Base> operator*(T left, const Vector4<Other, OtherBase>& right)
		{
			return Vector4<Base>(left * right.x, left * right.y, left * right.z, left * right.w);
		}

		template<typename Other, typename OtherBase>
		friend Vector4<Base> operator/(const Vector4<T>& left, const Vector4<Other, OtherBase>& right)
		{
			return left.Div(right);
		}

		friend Vector4<Base> operator/(const Vector4<T>& left, Base right)
		{
			return left.Div(right);
		}

		template<typename Other, typename OtherBase>
		friend Vector4<Base> operator/(T left, const Vector4<Other, OtherBase>& right)
		{
			return Vector4<Base>(left / right.x, left / right.y, left / right.z, left / right.w);
		}

		template<typename Other, typename OtherBase>
		Vector4<T>& operator+=(const Vector4<Other, OtherBase>& other)
		{
			*this = *this + other;
			return *this;
		}

		Vector4<T>& operator+=(Base other)
		{
			*this = *this + other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Vector4<T>& operator-=(const Vector4<Other, OtherBase>& other)
		{
			*this = *this - other;
			return *this;
		}

		Vector4<T>& operator-=(Base other)
		{
			*this = *this - other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Vector4<T>& operator*=(const Vector4<Other, OtherBase>& other)
		{
			*this = *this * other;
			return *this;
		}

		Vector4<T>& operator*=(Base other)
		{
			*this = *this * other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Vector4<T>& operator/=(const Vector4<Other, OtherBase>& other)
		{
			*this = *this / other;
			return *this;
		}

		Vector4<T>& operator/=(Base other)
		{
			*this = *this / other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		bool operator==(const Vector4<Other, OtherBase>& other) const
		{
			return (x == other.x && y == other.y && z == other.z && w == other.w);
		}

		template<typename Other, typename OtherBase>
		bool operator!=(const Vector4<Other, OtherBase>& other) const
		{
			return !(*this == other);
		}

		Vector4<Base> operator-() const
		{
			return Vector4<Base>(-x, -y, -z, w);
		}

		std::string ToString() const
		{
			return "Vector4(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
		}

		friend std::ostream& operator<<(std::ostream& stream, const Vector4<T, Base>& vector)
		{
			stream << vector.ToString();
			return stream;
		}

		Vector3<Base> xyz() const
		{
			return Vector3<Base>(x, y, z);
		}

		Vector3<T&, Base> xyz()
		{
			return Vector3<T&, Base>(x, y, z);
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
		static Base Distance(const Vector4<T>& left, const Vector4<Other, OtherBase>& right)
		{
			return (left - right).Length();
		}

		static Vector4<Base> Right()
		{
			return Vector4<Base>((Base)1, (Base)0, (Base)0, (Base)0);
		}

		static Vector4<Base> Up()
		{
			return Vector4<Base>((Base)0, (Base)1, (Base)0, (Base)0);
		}

		static Vector4<Base> Forward()
		{
			return Vector4<Base>((Base)0, (Base)0, (Base)1, (Base)0);
		}

	};

	using Vector4f = Vector4<float>;
	using Vector4i = Vector4<int>;
	using Vector4d = Vector4<double>;

	using Vector4fr = Vector4<float&, float>;
	using Vector4ir = Vector4<int&, int>;
	using Vector4dr = Vector4<double&, double>;
}