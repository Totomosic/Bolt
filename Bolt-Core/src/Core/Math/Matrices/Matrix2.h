#pragma once
#include "../Vectors/Vector2.h"
#include "Core/Profiling/Profiling.h"

namespace Bolt
{

	template<typename T, typename Base = T>
	struct BLT_API Matrix2
	{
	public:
		// Values stored in column major ie column 0 = 0, 1, 2
		union
		{
			T values[4];
			struct
			{
				T m00;
				T m10;
				T m01;
				T m11;
			};
		};

	public:
		Matrix2()
		{
			for (int i = 0; i < 4; i++)
			{
				values[i] = (T)0.0;
			}
		}

		Matrix2(T vals[9])
		{
			memcpy(values, vals, 4 * sizeof(T));
		}

		Matrix2(T m00, T m10, T m01, T m11)
			: m00(m00), m10(m10), m01(m01), m11(m11)
		{

		}

		template<typename Other, typename OtherBase>
		Matrix2(const Vector2<Other, OtherBase>& diagonal) : Matrix2()
		{
			values[0 * 2 + 0] = diagonal.x;
			values[1 * 2 + 1] = diagonal.y;
		}

		Matrix2(T diagonal) : Matrix2(Vector2<T, Base>(diagonal, diagonal))
		{

		}

		Vector2<T, Base> Column(int index) const
		{
			BLT_ASSERT(index < 2, "Index out of range");
			return Vector2<T, Base>(values[i * 2 + 0], values[i * 2 + 1]);
		}

		Vector2<T&, Base> Column(int index)
		{
			BLT_ASSERT(index < 2, "Index out of range");
			return Vector2<T&, Base>(values[i * 2 + 0], values[i * 2 + 1]);
		}

		Vector2<T, Base> Row(int index) const
		{
			BLT_ASSERT(index < 2, "Index out of range");
			return Vector3<T, Base>(values[0 * 3 + i], values[1 * 3 + i]);
		}

		Vector2<T&, Base> Row(int index)
		{
			BLT_ASSERT(index < 2, "Index out of range");
			return Vector2<T&, Base>(values[0 * 2 + i], values[1 * 2 + i]);
		}

		const T Element(int index) const
		{
			BLT_ASSERT(index < 4, "Index out of range");
			return values[index];
		}

		T& Element(int index)
		{
			BLT_ASSERT(index < 4, "Index out of range");
			return values[index];
		}

		const T Element(int row, int col) const
		{
			BLT_ASSERT(row < 2 && col < 2, "Index out of range");
			return values[col * 2 + row];
		}

		T& Element(int row, int col)
		{
			BLT_ASSERT(row < 2 && col < 2, "Index out of range");
			return values[col * 2 + row];
		}

		const T operator[](int index) const
		{
			return Element(index);
		}

		T& operator[](int index)
		{
			return Element(index);
		}

		template<typename Other, typename OtherBase>
		void SetColumn(int index, const Vector2<Other, OtherBase>& column)
		{
			values[index * 2 + 0] = column.x;
			values[index * 2 + 1] = column.y;
		}

		template<typename Other, typename OtherBase>
		void SetRow(int index, const Vector2<Other, OtherBase>& row)
		{
			values[0 * 2 + index] = row.x;
			values[1 * 2 + index] = row.y;
		}

		Matrix2<Base> Inverse() const
		{
			Base det = Element(0, 0) * Element(1, 1) - Element(0, 1) * Element(1, 0);
			if (det == (Base)0)
			{
				BLT_ASSERT(false, "Matrix could not be inverted");
				return Identity();
			}
			Base invdet = (Base)1.0 / det;

			Matrix2<Base> result;
			result.Element(0, 0) = Element(1, 1);
			result.Element(0, 1) = -Element(0, 1);
			result.Element(1, 0) = -Element(1, 0);
			result.Element(1, 1) = Element(0, 0);
			result *= invdet;
			return result;
		}

		Matrix2<Base> operator-() const
		{
			return *this * (Base)-1;
		}

		template<typename Other, typename OtherBase>
		friend Matrix2<Base> operator+(const Matrix2<T, Base>& left, const Matrix2<Other, OtherBase>& right)
		{
			Matrix2<Base> result;
			for (int i = 0; i < 4; i++)
			{
				result.values[i] = left.values[i] + right.values[i];
			}
			return result;
		}

		friend Matrix2<Base> operator+(const Matrix2<T, Base>& left, Base right)
		{
			Matrix2<Base> result;
			for (int i = 0; i < 4; i++)
			{
				result.values[i] = left.values[i] + right;
			}
			return result;
		}

		friend Matrix2<Base> operator+(Base left, const Matrix2<T, Base>& right)
		{
			return right + left;
		}

		template<typename Other, typename OtherBase>
		friend Matrix2<Base> operator-(const Matrix2<T, Base>& left, const Matrix2<Other, OtherBase>& right)
		{
			return left + (-right);
		}

		friend Matrix2<Base> operator-(const Matrix2<T, Base>& left, Base right)
		{
			return left + (-right);
		}

		friend Matrix2<Base> operator-(Base left, const Matrix2<T, Base>& right)
		{
			return left + (-right);
		}

		template<typename Other, typename OtherBase>
		friend Matrix2<Base> operator*(const Matrix2<T, Base>& left, const Matrix2<Other, OtherBase>& right)
		{
			Matrix2<T> result;
			for (int y = 0; y < 2; y++)
			{
				for (int x = 0; x < 2; x++)
				{
					T sum = (T)0;
					for (int e = 0; e < 2; e++)
					{
						sum += left.values[x + e * 2] * right.values[e + y * 2];
					}
					result.values[x + y * 2] = sum;
				}
			}
			return result;
		}

		friend Matrix2<Base> operator*(const Matrix2<T, Base>& left, Base right)
		{
			Matrix2<Base> result;
			for (int i = 0; i < 4; i++)
			{
				result.values[i] = left.values[i] * right;
			}
			return result;
		}

		friend Matrix2<Base> operator*(Base left, const Matrix2<T, Base>& right)
		{
			return right * left;
		}

		template<typename Other, typename OtherBase>
		friend Vector2<Base> operator*(const Matrix2<T, Base>& left, const Vector2<Other, OtherBase>& right)
		{
			return Vector2<Base>(left.m00 * right.x + left.m01 * right.y, left.m10 * right.x + left.m11 * right.y);
		}

		template<typename Other, typename OtherBase>
		Matrix2<T, Base>& operator+=(const Matrix2<Other, OtherBase>& other)
		{
			*this = *this + other;
			return *this;
		}

		Matrix2<T, Base>& operator+=(Base other)
		{
			*this = *this + other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Matrix2<T, Base>& operator-=(const Matrix2<Other, OtherBase>& other)
		{
			*this = *this - other;
			return *this;
		}

		Matrix2<T, Base>& operator-=(Base other)
		{
			*this = *this - other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Matrix2<T, Base>& operator*=(const Matrix2<Other, OtherBase>& other)
		{
			*this = *this * other;
			return *this;
		}

		Matrix2<T, Base>& operator*=(Base other)
		{
			*this = *this * other;
			return *this;
		}

		blt::string ToString() const
		{
			blt::string s = " ";
			blt::string result = "|" + std::to_string(values[0 + 0 * 2]) + s + std::to_string(values[0 + 1 * 2]) + "|" + "\n"
				+ "|" + std::to_string(values[1 + 0 * 2]) + s + std::to_string(values[1 + 1 * 2]) + "|";
			return result;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Matrix2<T, Base>& matrix)
		{
			stream << matrix.ToString();
			return stream;
		}

		void Transfer(XMLserializer& backend, bool isWriting)
		{
			BLT_TRANSFER(backend, m00);
			BLT_TRANSFER(backend, m10);
			BLT_TRANSFER(backend, m01);
			BLT_TRANSFER(backend, m11);
		}

		template<typename Other, typename OtherBase>
		bool operator==(const Matrix2<Other, OtherBase>& other) const
		{
			for (int i = 0; i < 4; i++)
			{
				if (values[i] != other.values[i])
				{
					return false;
				}
			}
			return true;
		}

		template<typename Other, typename OtherBase>
		bool operator!=(const Matrix2<Other, OtherBase>& other) const
		{
			return !(*this == other);
		}

		static Matrix2<T, Base> Identity()
		{
			return Matrix2<T, Base>((T)1.0);
		}

		static Matrix2<T, Base> Zero()
		{
			return Matrix2<T, Base>();
		}

		template<typename Other, typename OtherBase>
		static Matrix2<T, Base> FromColumns(const Vector2<Other, OtherBase>& c0, const Vector2<Other, OtherBase>& c1)
		{
			Matrix2<T, Base> result;
			result.SetColumn(0, c0);
			result.SetColumn(1, c1);
			return result;
		}

		template<typename Other, typename OtherBase>
		static Matrix2<T, Base> FromRows(const Vector2<Other, OtherBase>& r0, const Vector2<Other, OtherBase>& r1)
		{
			Matrix2<T, Base> result;
			result.SetRow(0, r0);
			result.SetRow(1, r1);
			return result;
		}

		template<typename Other, typename OtherBase>
		static Matrix2<T, Base> Rotation(T angle)
		{
			Matrix2<T> result = Identity();
			result.m00 = cos(angle);
			result.m10 = sin(angle);
			result.m01 = -sin(angle);
			result.m11 = cos(angle);
			return result;
		}

		template<typename Other, typename OtherBase>
		static Matrix2<T, Base> LinearInterpolate(const Matrix2<T, Base>& a, const Matrix2<Other, OtherBase>& b, T prop)
		{
			Matrix2<T, Base> result;
			for (int i = 0; i < 4; i++) {
				result.values[i] = a.values[i] * (1 - prop) + b.values[i] * prop;
			}
			return result;
		}

	};

	typedef Matrix2<float, float> Matrix2f;
	typedef Matrix2<int, int> Matrix2i;

}