#pragma once
#include "Logging.h"
#include "BoltLib/BoltTL/string.h"

namespace Bolt
{

	template<typename T, typename Base = T>
	struct BLT_API Vector2
	{
	public:
		T x;
		T y;

	public:
		Vector2(T x, T y)	
			: x(x), y(y)	
		{	
				
		}	
				
		Vector2(T value) : Vector2(value, value)	
		{	
				
		}	
				
		Vector2() : Vector2((T)0)	
		{	
				
		}	

		template<typename Other, typename OtherBase>
		Vector2(const Vector2<Other, OtherBase>& other) : Vector2((T)other.x, (T)other.y)
		{

		}

		template<typename Other, typename OtherBase>
		Vector2<T, Base>& operator=(const Vector2<Other, OtherBase>& other)
		{
			x = (T)other.x;
			y = (T)other.y;
			return *this;
		}

		void Clear()
		{
			x = (Base)0.0;
			y = (Base)0.0;
		}
				
		const T Get(int index) const	
		{	
			switch (index)	
			{	
			case 0:	
				return x;	
			case 1:	
				return y;	
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
			return x * x + y * y;	
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
		Base Dot(const Vector2<Other, OtherBase>& other) const	
		{	
			return x * (Base)other.x + y * (Base)other.y;
		}	
					
		Vector2<Base> Normalize() const
		{	
			return (*this) / Length();	
		}	
						
		template<typename Other, typename OtherBase>
		Vector2<Base> Add(const Vector2<Other, OtherBase>& other) const
		{	
			return Vector2<Base>(x + other.x, y + other.y);
		}	
						
		Vector2<Base> Add(Base other) const
		{	
			return Vector2<Base>(x + other, y + other);
		}	
						
		template<typename Other, typename OtherBase>
		Vector2<Base> Sub(const Vector2<Other, OtherBase>& other) const
		{	
			return Vector2<Base>(x - other.x, y - other.y);
		}	
						
		Vector2<Base> Sub(Base other) const
		{	
			return Vector2<Base>(x - other, y - other);
		}	
						
		template<typename Other, typename OtherBase>
		Vector2<Base> Mul(const Vector2<Other, OtherBase>& other) const
		{	
			return Vector2<Base>(x * other.x, y * other.y);
		}	
						
		Vector2<Base> Mul(Base other) const
		{	
			return Vector2<Base>(x * other, y * other);
		}	
						
		template<typename Other, typename OtherBase>
		Vector2<Base> Div(const Vector2<Other, OtherBase>& other) const
		{	
			return Vector2<Base>(x / other.x, y / other.y);
		}	
						
		Vector2<Base> Div(Base other) const
		{	
			return Vector2<Base>(x / other, y / other);
		}	
						
		template<typename Other, typename OtherBase>
		friend Vector2<Base> operator+(const Vector2<T, Base>& left, const Vector2<Other, OtherBase>& right)
		{	
			return left.Add(right);	
		}	
						
		friend Vector2<Base> operator+(const Vector2<T, Base>& left, Base right)
		{	
			return left.Add(right);	
		}	
						
		template<typename Other, typename OtherBase>
		friend Vector2<Base> operator+(T left, const Vector2<Other, OtherBase>& right)
		{	
			return Vector2<Base>(left + right.x, left + right.y);
		}	
						
		template<typename Other, typename OtherBase>
		friend Vector2<Base> operator-(const Vector2<T, Base>& left, const Vector2<Other, OtherBase>& right)
		{	
			return left.Sub(right);	
		}	
						
		friend Vector2<Base> operator-(const Vector2<T, Base>& left, Base right)
		{	
			return left.Sub(right);	
		}	
						
		template<typename Other, typename OtherBase>
		friend Vector2<Base> operator-(T left, const Vector2<Other, OtherBase>& right)
		{	
			return Vector2<Base>(left - right.x, left - right.y);
		}	
				
		template<typename Other, typename OtherBase>
		friend Vector2<Base> operator*(const Vector2<T, Base>& left, const Vector2<Other, OtherBase>& right)
		{	
			return left.Mul(right);	
		}	
						
		friend Vector2<Base> operator*(const Vector2<T, Base>& left, Base right)
		{	
			return left.Mul(right);	
		}	
						
		template<typename Other, typename OtherBase>
		friend Vector2<Base> operator*(T left, const Vector2<Other, OtherBase>& right)
		{	
			return Vector2<Base>(left * right.x, left * right.y);
		}	
						
		template<typename Other, typename OtherBase>
		friend Vector2<Base> operator/(const Vector2<T, Base>& left, const Vector2<Other, OtherBase>& right)
		{	
			return left.Div(right);	
		}	
						
		friend Vector2<Base> operator/(const Vector2<T, Base>& left, Base right)
		{	
			return left.Div(right);	
		}	
						
		template<typename Other, typename OtherBase>
		friend Vector2<Base> operator/(T left, const Vector2<Other, OtherBase>& right)
		{	
			return Vector2<Base>(left / right.x, left / right.y);
		}	
						
		template<typename Other, typename OtherBase>
		Vector2<T>& operator+=(const Vector2<Other, OtherBase>& other)
		{	
			*this = *this + other;	
			return *this;	
		}	
						
		Vector2<T>& operator+=(Base other)
		{	
			*this = *this + other;	
			return *this;	
		}	
						
		template<typename Other, typename OtherBase>
		Vector2<T>& operator-=(const Vector2<Other, OtherBase>& other)
		{	
			*this = *this - other;	
			return *this;	
		}	
						
		Vector2<T>& operator-=(Base other)
		{	
			*this = *this - other;	
			return *this;	
		}	
						
		template<typename Other, typename OtherBase>
		Vector2<T>& operator*=(const Vector2<Other, OtherBase>& other)
		{	
			*this = *this * other;	
			return *this;	
		}	
						
		Vector2<T>& operator*=(Base other)
		{	
			*this = *this * other;	
			return *this;	
		}	
						
		template<typename Other, typename OtherBase>
		Vector2<T>& operator/=(const Vector2<Other, OtherBase>& other)
		{	
			*this = *this / other;	
			return *this;	
		}	
						
		Vector2<T>& operator/=(Base other)
		{	
			*this = *this / other;	
			return *this;	
		}	
						
		template<typename Other, typename OtherBase>
		bool operator==(const Vector2<Other, OtherBase>& other) const
		{	
			return (x == other.x && y == other.y);	
		}	
					
		template<typename Other, typename OtherBase>
		bool operator!=(const Vector2<Other, OtherBase>& other) const
		{	
			return !(*this == other);	
		}	
						
		Vector2<T> operator-() const	
		{	
			return Vector2<T, Base>(-x, -y);	
		}	
						
		blt::string ToString() const
		{	
			return "Vector2(" + std::to_string(x) + ", " + std::to_string(y) + ")";	
		}	
						
		friend std::ostream& operator<<(std::ostream& stream, const Vector2<T, Base>& vector)
		{	
			stream << vector.ToString();	
			return stream;	
		}	
						
		static Base Distance(const Vector2<Base>& left, const Vector2<Base>& right)
		{	
			return (left - right).Length();	
		}	

		template<typename Other, typename OtherBase>
		static Vector2<Base> Dot(const Vector2<T, Base>& left, const Vector2<Other, OtherBase>& right)
		{
			return left.Dot(right);
		}
						
		static Vector2<Base> Right()
		{	
			return Vector2<Base>((Base)1, (Base)0);
		}	
						
		static Vector2<Base> Up()
		{	
			return Vector2<Base>((Base)0, (Base)1);
		}
	};

	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;
	using Vector2d = Vector2<double>;

	using Vector2fr = Vector2<float&, float>;
	using Vector2ir = Vector2<int&, int>;
	using Vector2dr = Vector2<double&, double>;

}