#pragma once
#include "../Vectors/Vector3.h"
#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	template<typename T, typename Base = T>
	struct BLT_API Matrix3
	{
	public:
		// Values stored in column major ie column 0 = 0, 1, 2
		union
		{
			T values[9];
			struct
			{
				T m00;
				T m10;
				T m20;
				T m01;
				T m11;
				T m21;
				T m02;
				T m12;
				T m22;
			};
		};

	public:
		Matrix3()
		{
			for (int i = 0; i < 9; i++)
			{
				values[i] = (T)0.0;
			}
		}

		Matrix3(T vals[9])
		{
			memcpy(values, vals, 9 * sizeof(T));
		}

		Matrix3(T m00, T m10, T m20, T m01, T m11, T m21, T m02, T m12, T m22)
			: m00(m00), m10(m10), m20(m20), m01(m01), m11(m11), m21(m21), m02(m02), m12(m12), m22(m22)
		{
				
		}

		template<typename Other, typename OtherBase>
		Matrix3(const Vector3<Other, OtherBase>& diagonal) : Matrix3()
		{
			values[0 * 3 + 0] = diagonal.x;
			values[1 * 3 + 1] = diagonal.y;
			values[2 * 3 + 2] = diagonal.z;
		}

		Matrix3(T diagonal) : Matrix3(Vector3<T, Base>(diagonal, diagonal, diagonal))
		{
			
		}

		Vector3<T, Base> Column(int index) const
		{
			BLT_ASSERT(index < 3, "Index out of range");
			return Vector3<T, Base>(values[index * 3 + 0], values[index * 3 + 1], values[index * 3 + 2]);
		}

		Vector3<T&, Base> Column(int index)
		{
			BLT_ASSERT(index < 3, "Index out of range");
			return Vector3<T&, Base>(values[index * 3 + 0], values[index * 3 + 1], values[index * 3 + 2]);
		}

		Vector3<T, Base> Row(int index) const
		{
			BLT_ASSERT(index < 3, "Index out of range");
			return Vector3<T, Base>(values[0 * 3 + index], values[1 * 3 + index], values[2 * 3 + index]);
		}

		Vector3<T&, Base> Row(int index)
		{
			BLT_ASSERT(index < 3, "Index out of range");
			return Vector3<T&, Base>(values[0 * 3 + index], values[1 * 3 + index], values[2 * 3 + index]);
		}

		const T Element(int index) const
		{
			BLT_ASSERT(index < 9, "Index out of range");
			return values[index];
		}

		T& Element(int index)
		{
			BLT_ASSERT(index < 9, "Index out of range");
			return values[index];
		}

		const T Element(int row, int col) const
		{
			BLT_ASSERT(row < 3 && col < 3, "Index out of range");
			return values[col * 3 + row];
		}

		T& Element(int row, int col)
		{
			BLT_ASSERT(row < 3 && col < 3, "Index out of range");
			return values[col * 3 + row];
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
		void SetColumn(int index, const Vector3<Other, OtherBase>& column)
		{
			values[index * 3 + 0] = column.x;
			values[index * 3 + 1] = column.y;
			values[index * 3 + 2] = column.z;
		}

		template<typename Other, typename OtherBase>
		void SetRow(int index, const Vector3<Other, OtherBase>& row)
		{
			values[0 * 3 + index] = row.x;
			values[1 * 3 + index] = row.y;
			values[2 * 3 + index] = row.z;
		}

		Matrix3<Base> Inverse() const
		{
			Base det = Element(0, 0) * (Element(1, 1) * Element(2, 2) - Element(2, 1) * Element(1, 2)) -
				Element(0, 1) * (Element(1, 0) * Element(2, 2) - Element(1, 2) * Element(2, 0)) +
				Element(0, 2) * (Element(1, 0) * Element(2, 1) - Element(1, 1) * Element(2, 0));

			Base invdet = (Base)1.0 / det;

			Matrix3<Base> result;
			result.Element(0, 0) = (Element(1, 1) * Element(2, 2) - Element(2, 1) * Element(1, 2)) * invdet;
			result.Element(0, 1) = (Element(0, 2) * Element(2, 1) - Element(0, 1) * Element(2, 2)) * invdet;
			result.Element(0, 2) = (Element(0, 1) * Element(1, 2) - Element(0, 2) * Element(1, 1)) * invdet;
			result.Element(1, 0) = (Element(1, 2) * Element(2, 0) - Element(1, 0) * Element(2, 2)) * invdet;
			result.Element(1, 1) = (Element(0, 0) * Element(2, 2) - Element(0, 2) * Element(2, 0)) * invdet;
			result.Element(1, 2) = (Element(1, 0) * Element(0, 2) - Element(0, 0) * Element(1, 2)) * invdet;
			result.Element(2, 0) = (Element(1, 0) * Element(2, 1) - Element(2, 0) * Element(1, 1)) * invdet;
			result.Element(2, 1) = (Element(2, 0) * Element(0, 1) - Element(0, 0) * Element(2, 1)) * invdet;
			result.Element(2, 2) = (Element(0, 0) * Element(1, 1) - Element(1, 0) * Element(0, 1)) * invdet;
			return result;
		}

		Matrix3<Base> operator-() const
		{
			return *this * (Base)-1;
		}

		template<typename Other, typename OtherBase>
		friend Matrix3<Base> operator+(const Matrix3<T, Base>& left, const Matrix3<Other, OtherBase>& right)
		{
			Matrix3<Base> result;
			for (int i = 0; i < 9; i++)
			{
				result.values[i] = left.values[i] + right.values[i];
			}
			return result;
		}

		friend Matrix3<Base> operator+(const Matrix3<T, Base>& left, Base right)
		{
			Matrix3<Base> result;
			for (int i = 0; i < 9; i++)
			{
				result.values[i] = left.values[i] + right;
			}
			return result;
		}

		friend Matrix3<Base> operator+(Base left, const Matrix3<T, Base>& right)
		{
			return right + left;
		}

		template<typename Other, typename OtherBase>
		friend Matrix3<Base> operator-(const Matrix3<T, Base>& left, const Matrix3<Other, OtherBase>& right)
		{
			return left + (-right);
		}

		friend Matrix3<Base> operator-(const Matrix3<T, Base>& left, Base right)
		{
			return left + (-right);
		}

		friend Matrix3<Base> operator-(Base left, const Matrix3<T, Base>& right)
		{
			return left + (-right);
		}

		template<typename Other, typename OtherBase>
		friend Matrix3<Base> operator*(const Matrix3<T, Base>& left, const Matrix3<Other, OtherBase>& right)
		{
			Matrix3<T> result;
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					T sum = (T)0;
					for (int e = 0; e < 3; e++)
					{
						sum += left.values[x + e * 3] * right.values[e + y * 3];
					}
					result.values[x + y * 3] = sum;
				}
			}
			return result;
		}

		friend Matrix3<Base> operator*(const Matrix3<T, Base>& left, Base right)
		{
			Matrix3<Base> result;
			for (int i = 0; i < 9; i++)
			{
				result.values[i] = left.values[i] * right;
			}
			return result;
		}

		friend Matrix3<Base> operator*(Base left, const Matrix3<T, Base>& right)
		{
			return right * left;
		}

		template<typename Other, typename OtherBase>
		friend Vector3<Base> operator*(const Matrix3<T, Base>& left, const Vector3<Other, OtherBase>& right)
		{
			Vector3<Base> result;
			result.x = left.Element(0, 0) * right.x + left.Element(0, 1) * right.y + left.Element(0, 2) * right.z;
			result.y = left.Element(1, 0) * right.x + left.Element(1, 1) * right.y + left.Element(1, 2) * right.z;
			result.z = left.Element(2, 0) * right.x + left.Element(2, 1) * right.y + left.Element(2, 2) * right.z;
			return result;
		}

		template<typename Other, typename OtherBase>
		Matrix3<T, Base>& operator+=(const Matrix3<Other, OtherBase>& other)
		{
			*this = *this + other;
			return *this;
		}

		Matrix3<T, Base>& operator+=(Base other)
		{
			*this = *this + other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Matrix3<T, Base>& operator-=(const Matrix3<Other, OtherBase>& other)
		{
			*this = *this - other;
			return *this;
		}

		Matrix3<T, Base>& operator-=(Base other)
		{
			*this = *this - other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Matrix3<T, Base>& operator*=(const Matrix3<Other, OtherBase>& other)
		{
			*this = *this * other;
			return *this;
		}

		Matrix3<T, Base>& operator*=(Base other)
		{
			*this = *this * other;
			return *this;
		}

		blt::string ToString() const
		{
			blt::string s = " ";
			blt::string result = "|" + std::to_string(values[0 + 0 * 3]) + s + std::to_string(values[0 + 1 * 3]) + s + std::to_string(values[0 + 2 * 3]) + "|" + "\n"
				+ "|" + std::to_string(values[1 + 0 * 3]) + s + std::to_string(values[1 + 1 * 3]) + s + std::to_string(values[1 + 2 * 3]) + "|" + "\n"
				+ "|" + std::to_string(values[2 + 0 * 3]) + s + std::to_string(values[2 + 1 * 3]) + s + std::to_string(values[2 + 2 * 3]) + "|";
			return result;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Matrix3<T, Base>& matrix)
		{
			stream << matrix.ToString();
			return stream;
		}

		template<typename Other, typename OtherBase>
		bool operator==(const Matrix3<Other, OtherBase>& other) const
		{
			for (int i = 0; i < 9; i++)
			{
				if (values[i] != other.values[i])
				{
					return false;
				}
			}
			return true;
		}

		template<typename Other, typename OtherBase>
		bool operator!=(const Matrix3<Other, OtherBase>& other) const
		{
			return !(*this == other);
		}

		static Matrix3<T, Base> Identity()
		{
			return Matrix3<T, Base>((T)1.0);
		}

		static Matrix3<T, Base> Zero()
		{
			return Matrix3<T, Base>();
		}

		template<typename Other, typename OtherBase>
		static Matrix3<T, Base> FromColumns(const Vector3<Other, OtherBase>& c0, const Vector3<Other, OtherBase>& c1, const Vector3<Other, OtherBase>& c2)
		{
			Matrix3<T, Base> result;
			result.SetColumn(0, c0);
			result.SetColumn(1, c1);
			result.SetColumn(2, c2);
			return result;
		}

		template<typename Other, typename OtherBase>
		static Matrix3<T, Base> FromRows(const Vector3<Other, OtherBase>& r0, const Vector3<Other, OtherBase>& r1, const Vector3<Other, OtherBase>& r2)
		{
			Matrix3<T, Base> result;
			result.SetRow(0, r0);
			result.SetRow(1, r1);
			result.SetRow(2, r2);
			return result;
		}

		template<typename Other, typename OtherBase>
		static Matrix3<T, Base> Scale(const Vector3<Other, OtherBase>& scale)
		{
			Matrix3<T, Base> result = Identity();
			result.Element(0, 0) = scale.x;
			result.Element(1, 1) = scale.y;
			result.Element(2, 2) = scale.z;
			return result;
		}

		static Matrix3<T, Base> Scale(Base x, Base y, Base z)
		{
			return Scale(Vector3<Base>(x, y, z));
		}

		template<typename Other, typename OtherBase>
		static Matrix3<T, Base> Translation(const Vector3<Other, OtherBase>& translation)
		{
			Matrix3<T, Base> result = Identity();
			result.SetColumn(2, translation);
			return result;
		}

		static Matrix3<T, Base> Translation(Base x, Base y, Base z)
		{
			return Translation(Vector3<Base>(x, y, z));
		}

		template<typename Other, typename OtherBase>
		static Matrix3<T, Base> Translation(const Vector2<Other, OtherBase>& translation)
		{
			return Translation(Vector3<Other, OtherBase>(translation.x, translation.y, (Other)1));
		}

		static Matrix3<T, Base> Translation(Base x, Base y)
		{
			return Translation(Vector2<Base>(x, y));
		}

		template<typename Other, typename OtherBase>
		static Matrix3<T, Base> Rotation(T angle, const Vector3<Other, OtherBase>& axis)
		{
			Matrix3<T> result = Identity();

			T c = (T)cos(angle);
			T s = (T)sin(angle);
			T omc = (T)1 - c;

			T x = axis.x;
			T y = axis.y;
			T z = axis.z;

			result.values[0 + 0 * 3] = x * omc + c;
			result.values[1 + 0 * 3] = y * x * omc + z * s;
			result.values[2 + 0 * 3] = x * z * omc - y * s;

			result.values[0 + 1 * 3] = x * y * omc - z * s;
			result.values[1 + 1 * 3] = y * omc + c;
			result.values[2 + 1 * 3] = y * z * omc + x * s;

			result.values[0 + 2 * 3] = x * z * omc + y * s;
			result.values[1 + 2 * 3] = y * z * omc - x * s;
			result.values[2 + 2 * 3] = z * omc + c;

			return result;
		}

		template<typename Other, typename OtherBase>
		static Matrix3<T, Base> LinearInterpolate(const Matrix3<T, Base>& a, const Matrix3<Other, OtherBase>& b, T prop)
		{
			Matrix3<T, Base> result;
			for (int i = 0; i < 9; i++) {
				result.values[i] = a.values[i] * (1 - prop) + b.values[i] * prop;
			}
			return result;
		}

	};

	using Matrix3f = Matrix3<float>;
	using Matrix3i = Matrix3<int>;

}