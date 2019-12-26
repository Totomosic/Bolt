#pragma once
#include "../Vectors/Vector4.h"
#include "Matrix3.h"
#include "../Frustum.h"

namespace Bolt
{

	template<typename T, typename Base = T>
	struct BLT_API Matrix4
	{
	public:
		// Values stored in column major ie column 0 = 0, 1, 2, 3
		union
		{
			T values[16];
			struct
			{
				T m00;
				T m10;
				T m20;
				T m30;
				T m01;
				T m11;
				T m21;
				T m31;
				T m02;
				T m12;
				T m22;
				T m32;
				T m03;
				T m13;
				T m23;
				T m33;
			};
		};

	public:
		Matrix4(T elements[16])	
		{	
			memcpy(values, elements, 16 * sizeof(T));
		}	
				
		template<typename Other, typename OtherBase>
		Matrix4(const Vector4<Other, OtherBase>& diagonal) : Matrix4()	
		{	
			values[4 * 0 + 0] = diagonal.x;	
			values[4 * 1 + 1] = diagonal.y;	
			values[4 * 2 + 2] = diagonal.z;	
			values[4 * 3 + 3] = diagonal.w;	
		}	
				
		Matrix4(T diagonal) : Matrix4(Vector4<T>(diagonal, diagonal, diagonal, diagonal))	
		{	
				
		}	
				
		Matrix4()
			: values()
		{	
			for (int i = 0; i < 16; i++)	
			{	
				values[i] = (T)0;	
			}	
		}	

		template<typename Other, typename OtherBase>
		Matrix4(const Matrix3<Other, OtherBase>& mat3) : Matrix4((T)1.0)
		{
			Element(0, 0) = mat3.Element(0, 0);
			Element(1, 0) = mat3.Element(1, 0);
			Element(2, 0) = mat3.Element(2, 0);
			Element(0, 1) = mat3.Element(0, 1);
			Element(1, 1) = mat3.Element(1, 1);
			Element(2, 1) = mat3.Element(2, 1);
			Element(0, 2) = mat3.Element(0, 2);
			Element(1, 2) = mat3.Element(1, 2);
			Element(2, 2) = mat3.Element(2, 2);
		}
				
		Vector4<T> Column(int index) const	
		{	
			BLT_ASSERT(index < 4, "Index out of range");	
			return Vector4<T>(values[4 * index + 0], values[4 * index + 1], values[4 * index + 2], values[4 * index + 3]);	
		}	

		Vector4<T&, T> Column(int index)
		{
			BLT_ASSERT(index < 4, "Index out of range");
			return Vector4<T&, T>(values[4 * index + 0], values[4 * index + 1], values[4 * index + 2], values[4 * index + 3]);
		}
				
		Vector4<T> Row(int index) const	
		{	
			BLT_ASSERT(index < 4, "Index out of range");	
			return Vector4<T>(values[4 * 0 + index], values[4 * 1 + index], values[4 * 2 + index], values[4 * 3 + index]);	
		}	

		Vector4<T&, T> Row(int index)
		{
			BLT_ASSERT(index < 4, "Index out of range");
			return Vector4<T&, T>(values[4 * 0 + index], values[4 * 1 + index], values[4 * 2 + index], values[4 * 3 + index]);
		}
				
		const T Element(int index) const	
		{	
			BLT_ASSERT(index < 16, "Index out of range");	
			return values[index];	
		}	
				
		T& Element(int index)	
		{	
			BLT_ASSERT(index < 16, "Index out of range");	
			return values[index];	
		}	
				
		const T Element(int row, int col) const	
		{	
			BLT_ASSERT(row < 4 && col < 4, "Index out of range");	
			return values[4 * col + row];	
		}	
				
		T& Element(int row, int col)	
		{	
			BLT_ASSERT(row < 4 && col < 4, "Index out of range");	
			return values[4 * col + row];	
		}	
				
		const T operator[](int index) const	
		{	
			return Element(index);	
		}	
				
		T& operator[](int index)	
		{	
			return Element(index);	
		}	

		Matrix3<T, Base> Mat3() const
		{
			Matrix3<T, Base> result;
			result.Element(0, 0) = Element(0, 0);
			result.Element(0, 1) = Element(0, 1);
			result.Element(0, 2) = Element(0, 2);
			result.Element(1, 0) = Element(1, 0);
			result.Element(1, 1) = Element(1, 1);
			result.Element(1, 2) = Element(1, 2);
			result.Element(2, 0) = Element(2, 0);
			result.Element(2, 1) = Element(2, 1);
			result.Element(2, 2) = Element(2, 2);
			return result;
		}
				
		template<typename Other, typename OtherBase>
		void SetColumn(int index, const Vector4<Other, OtherBase>& column)	
		{	
			values[4 * index + 0] = column.x;	
			values[4 * index + 1] = column.y;	
			values[4 * index + 2] = column.z;	
			values[4 * index + 3] = column.w;	
		}	
				
		template<typename Other, typename OtherBase>
		void SetRow(int index, const Vector4<Other, OtherBase>& row)	
		{	
			values[4 * 0 + index] = row.x;	
			values[4 * 1 + index] = row.y;	
			values[4 * 2 + index] = row.z;	
			values[4 * 3 + index] = row.w;	
		}	
				
		void SetElement(int row, int col, T value)	
		{	
			values[4 * col + row] = value;	
		}	

		Matrix4<Base> Transpose() const
		{
			Vector4<Base> r0 = Row(0);
			Vector4<Base> r1 = Row(1);
			Vector4<Base> r2 = Row(2);
			Vector4<Base> r3 = Row(3);
			Matrix4<Base> result;
			result.SetColumn(0, r0);
			result.SetColumn(1, r1);
			result.SetColumn(2, r2);
			result.SetColumn(3, r3);
			return result;
		}
				
		Matrix4<Base> Inverse() const	
		{	
			Base inv[16];	
			Base invOut[16];	
			Base det;	
			int i;	
				
			inv[0] = values[5] * values[10] * values[15] - 
			values[5] * values[11] * values[14] - 
			values[9] * values[6] * values[15] + 
			values[9] * values[7] * values[14] + 
			values[13] * values[6] * values[11] - 
			values[13] * values[7] * values[10];	
				
			inv[4] = -values[4] * values[10] * values[15] + 
			values[4] * values[11] * values[14] + 
			values[8] * values[6] * values[15] - 
			values[8] * values[7] * values[14] - 
			values[12] * values[6] * values[11] + 
			values[12] * values[7] * values[10];	
				
			inv[8] = values[4] * values[9] * values[15] - 
			values[4] * values[11] * values[13] - 
			values[8] * values[5] * values[15] + 
			values[8] * values[7] * values[13] + 
			values[12] * values[5] * values[11] - 
			values[12] * values[7] * values[9];	
				
			inv[12] = -values[4] * values[9] * values[14] + 
			values[4] * values[10] * values[13] + 
			values[8] * values[5] * values[14] - 
			values[8] * values[6] * values[13] - 
			values[12] * values[5] * values[10] + 
			values[12] * values[6] * values[9];	
				
			inv[1] = -values[1] * values[10] * values[15] + 
			values[1] * values[11] * values[14] + 
			values[9] * values[2] * values[15] - 
			values[9] * values[3] * values[14] - 
			values[13] * values[2] * values[11] + 
			values[13] * values[3] * values[10];	
				
			inv[5] = values[0] * values[10] * values[15] - 
			values[0] * values[11] * values[14] - 
			values[8] * values[2] * values[15] + 
			values[8] * values[3] * values[14] + 
			values[12] * values[2] * values[11] - 
			values[12] * values[3] * values[10];	
				
			inv[9] = -values[0] * values[9] * values[15] + 
			values[0] * values[11] * values[13] + 
			values[8] * values[1] * values[15] - 
			values[8] * values[3] * values[13] - 
			values[12] * values[1] * values[11] + 
			values[12] * values[3] * values[9];	
				
			inv[13] = values[0] * values[9] * values[14] - 
			values[0] * values[10] * values[13] - 
			values[8] * values[1] * values[14] + 
			values[8] * values[2] * values[13] + 
			values[12] * values[1] * values[10] - 
			values[12] * values[2] * values[9];	
				
			inv[2] = values[1] * values[6] * values[15] - 
			values[1] * values[7] * values[14] - 
			values[5] * values[2] * values[15] + 
			values[5] * values[3] * values[14] + 
			values[13] * values[2] * values[7] - 
			values[13] * values[3] * values[6];	
				
			inv[6] = -values[0] * values[6] * values[15] + 
			values[0] * values[7] * values[14] + 
			values[4] * values[2] * values[15] - 
			values[4] * values[3] * values[14] - 
			values[12] * values[2] * values[7] + 
			values[12] * values[3] * values[6];	
				
			inv[10] = values[0] * values[5] * values[15] - 
			values[0] * values[7] * values[13] - 
			values[4] * values[1] * values[15] + 
			values[4] * values[3] * values[13] + 
			values[12] * values[1] * values[7] - 
			values[12] * values[3] * values[5];	
				
			inv[14] = -values[0] * values[5] * values[14] + 
			values[0] * values[6] * values[13] + 
			values[4] * values[1] * values[14] - 
			values[4] * values[2] * values[13] - 
			values[12] * values[1] * values[6] + 
			values[12] * values[2] * values[5];	
				
			inv[3] = -values[1] * values[6] * values[11] + 
			values[1] * values[7] * values[10] + 
			values[5] * values[2] * values[11] - 
			values[5] * values[3] * values[10] - 
			values[9] * values[2] * values[7] + 
			values[9] * values[3] * values[6];	
				
			inv[7] = values[0] * values[6] * values[11] - 
			values[0] * values[7] * values[10] - 
			values[4] * values[2] * values[11] + 
			values[4] * values[3] * values[10] + 
			values[8] * values[2] * values[7] - 
			values[8] * values[3] * values[6];	
				
			inv[11] = -values[0] * values[5] * values[11] + 
			values[0] * values[7] * values[9] + 
			values[4] * values[1] * values[11] - 
			values[4] * values[3] * values[9] - 
			values[8] * values[1] * values[7] + 
			values[8] * values[3] * values[5];	
				
			inv[15] = values[0] * values[5] * values[10] - 
			values[0] * values[6] * values[9] - 
			values[4] * values[1] * values[10] + 
			values[4] * values[2] * values[9] + 
			values[8] * values[1] * values[6] - 
			values[8] * values[2] * values[5];	
				
			det = values[0] * inv[0] + values[1] * inv[4] + values[2] * inv[8] + values[3] * inv[12];	
				
			if (det == (Base)0)	
			{	
				BLT_ASSERT(false, "Matrix could not be inverted");	
				return Identity();	
			}	
					
			det = (Base)1 / det;	
				
			for (i = 0; i < 16; i++)	
			{	
				invOut[i] = inv[i] * det;	
			}					
			return Matrix4<Base>(invOut);	
		}	
						
		Matrix4<Base> operator-() const	
		{	
			return *this * (Base)-1;	
		}	
				
		template<typename Other, typename OtherBase>
		friend Matrix4<Base> operator+(const Matrix4<T, Base>& left, const Matrix4<Other, OtherBase>& right)
		{	
			Matrix4<Base> result;	
			for (int i = 0; i < 16; i++)	
			{	
				result.values[i] = left.values[i] + right.values[i];	
			}	
			return result;	
		}	
				
		friend Matrix4<Base> operator+(const Matrix4<T, Base>& left, Base right)
		{	
			Matrix4<Base> result;	
			for (int i = 0; i < 16; i++)	
			{	
				result.values[i] = left.values[i] + right;	
			}	
			return result;	
		}	
				
		friend Matrix4<Base> operator+(Base left, const Matrix4<T, Base>& right)
		{	
			return right + left;	
		}	
				
		template<typename Other, typename OtherBase>
		friend Matrix4<Base> operator-(const Matrix4<T, Base>& left, const Matrix4<Other, OtherBase>& right)
		{	
			Matrix4<Base> result;	
			for (int i = 0; i < 16; i++)	
			{	
				result.values[i] = left.values[i] - right.values[i];	
			}	
			return result;	
		}	
				
		friend Matrix4<Base> operator-(const Matrix4<T, Base>& left, Base right)
		{	
			Matrix4<Base> result;	
			for (int i = 0; i < 16; i++)	
			{	
				result.values[i] = left.values[i] - right;	
			}	
			return result;	
		}	
				
		friend Matrix4<Base> operator-(Base left, const Matrix4<T, Base>& right)
		{	
			Matrix4<Base> result;	
			for (int i = 0; i < 16; i++)	
			{	
				result.values[i] = left - right.values[i];	
			}	
			return result;	
		}	
				
		template<typename Other, typename OtherBase>
		friend Matrix4<Base> operator*(const Matrix4<T, Base>& left, const Matrix4<Other, OtherBase>& right)
		{	
			Matrix4<Base> result;	
			for (int y = 0; y < 4; y++)	
			{	
				for (int x = 0; x < 4; x++)	
				{	
					T sum = (T)0;	
					for (int e = 0; e < 4; e++)	
					{	
						sum += left.values[x + e * 4] * right.values[e + y * 4];	
					}	
					result.values[x + y * 4] = sum;	
				}	
			}	
			return result;	
		}	
				
		friend Matrix4<Base> operator*(const Matrix4<T, Base>& left, Base right)
		{	
			Matrix4<Base> result;	
			for (int i = 0; i < 16; i++)	
			{	
				result.values[i] = left.values[i] * right;	
			}	
			return result;	
		}	
				
		friend Matrix4<Base> operator*(Base left, const Matrix4<T, Base>& right)
		{	
			return right * left;	
		}	
				
		template<typename Other, typename OtherBase>
		friend Vector4<OtherBase> operator*(const Matrix4<T, Base>& left, const Vector4<Other, OtherBase>& right)
		{	
			T x = left.values[4 * 0 + 0] * right.x + left.values[4 * 1 + 0] * right.y + left.values[4 * 2 + 0] * right.z + left.values[4 * 3 + 0] * right.w;	
			T y = left.values[4 * 0 + 1] * right.x + left.values[4 * 1 + 1] * right.y + left.values[4 * 2 + 1] * right.z + left.values[4 * 3 + 1] * right.w;	
			T z = left.values[4 * 0 + 2] * right.x + left.values[4 * 1 + 2] * right.y + left.values[4 * 2 + 2] * right.z + left.values[4 * 3 + 2] * right.w;	
			T w = left.values[4 * 0 + 3] * right.x + left.values[4 * 1 + 3] * right.y + left.values[4 * 2 + 3] * right.z + left.values[4 * 3 + 3] * right.w;	
			return Vector4<Other, OtherBase>(x, y, z, w);	
		}	
				
		template<typename Other, typename OtherBase>
		friend Vector3<OtherBase> operator*(const Matrix4<T, Base>& left, const Vector3<Other, OtherBase>& right)
		{	
			Vector4<OtherBase> result = left * Vector4<OtherBase>(right, (OtherBase)1);	
			return result.xyz(); 
		}	
				
		template<typename Other, typename OtherBase>
		friend Vector2<OtherBase> operator*(const Matrix4<T, Base>& left, const Vector2<Other, OtherBase>& right)
		{	
			Vector3<OtherBase> result = left * Vector3<Other, OtherBase>(right, (OtherBase)0);
			return result.xy();	
		}	

		template<typename Other, typename OtherBase>
		Matrix4<T, Base>& operator+=(const Matrix4<Other, OtherBase>& other)
		{
			*this = *this + other;
			return *this;
		}

		Matrix4<T, Base>& operator+=(Base other)
		{
			*this = *this + other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Matrix4<T, Base>& operator-=(const Matrix4<Other, OtherBase>& other)
		{
			*this = *this - other;
			return *this;
		}

		Matrix4<T, Base>& operator-=(Base other)
		{
			*this = *this - other;
			return *this;
		}

		template<typename Other, typename OtherBase>
		Matrix4<T, Base>& operator*=(const Matrix4<Other, OtherBase>& other)
		{
			*this = *this * other;
			return *this;
		}

		Matrix4<T, Base>& operator*=(Base other)
		{
			*this = *this * other;
			return *this;
		}
				
		blt::string ToString() const
		{	
			blt::string s = " ";
			blt::string result = "|" + std::to_string(values[0 + 0 * 4]) + s + std::to_string(values[0 + 1 * 4]) + s + std::to_string(values[0 + 2 * 4]) + s + std::to_string(values[0 + 3 * 4]) + "|" + "\n"
			+ "|" + std::to_string(values[1 + 0 * 4]) + s + std::to_string(values[1 + 1 * 4]) + s + std::to_string(values[1 + 2 * 4]) + s + std::to_string(values[1 + 3 * 4]) + "|" + "\n"	
			+ "|" + std::to_string(values[2 + 0 * 4]) + s + std::to_string(values[2 + 1 * 4]) + s + std::to_string(values[2 + 2 * 4]) + s + std::to_string(values[2 + 3 * 4]) + "|" + "\n"	
			+ "|" + std::to_string(values[3 + 0 * 4]) + s + std::to_string(values[3 + 1 * 4]) + s + std::to_string(values[3 + 2 * 4]) + s + std::to_string(values[3 + 3 * 4]) + "|";	
			return result;	
		}	
				
		friend std::ostream& operator<<(std::ostream& stream, const Matrix4<T, Base>& right)	
		{	
			stream << right.ToString();	
			return stream;	
		}	

		void Transfer(XMLserializer& backend, bool isWriting)
		{
			BLT_TRANSFER(backend, m00);
			BLT_TRANSFER(backend, m10);
			BLT_TRANSFER(backend, m20);
			BLT_TRANSFER(backend, m30);
			BLT_TRANSFER(backend, m01);
			BLT_TRANSFER(backend, m11);
			BLT_TRANSFER(backend, m21);
			BLT_TRANSFER(backend, m31);
			BLT_TRANSFER(backend, m02);
			BLT_TRANSFER(backend, m12);
			BLT_TRANSFER(backend, m22);
			BLT_TRANSFER(backend, m32);
			BLT_TRANSFER(backend, m03);
			BLT_TRANSFER(backend, m13);
			BLT_TRANSFER(backend, m23);
			BLT_TRANSFER(backend, m33);
		}
				
		static Matrix4<T, Base> Identity()
		{	
			return Matrix4<T, Base>((T)1);	
		}	
				
		static Matrix4<T, Base> Zero()
		{	
			return Matrix4<T, Base>((T)0);	
		}	
				
		static Matrix4<T, Base> FromColumns(const Vector4<T>& c0, const Vector4<T>& c1, const Vector4<T>& c2, const Vector4<T>& c3)
		{	
			Matrix4<T, Base> result;	
			result.SetColumn(0, c0);	
			result.SetColumn(1, c1);	
			result.SetColumn(2, c2);	
			result.SetColumn(3, c3);	
			return result;	
		}	
				
		static Matrix4<T, Base> FromRows(const Vector4<T>& r0, const Vector4<T>& r1, const Vector4<T>& r2, const Vector4<T>& r3)
		{	
			Matrix4<T, Base> result;	
			result.SetRow(0, r0);	
			result.SetRow(1, r1);	
			result.SetRow(2, r2);	
			result.SetRow(3, r3);	
			return result;	
		}	
				
		static Matrix4<T, Base> Orthographic(T left, T right, T bottom, T top, T nearPlane, T farPlane)
		{	
			Matrix4<T, Base> result = Identity();	
				
			result.values[0 + 0 * 4] = (T)2 / (right - left);	
			result.values[1 + 1 * 4] = (T)2 / (top - bottom);	
			result.values[2 + 2 * 4] = (T)-2 / (farPlane - nearPlane);	
				
			result.values[0 + 3 * 4] = -(left + right) / (right - left);	
			result.values[1 + 3 * 4] = -(bottom + top) / (top - bottom);	
			result.values[2 + 3 * 4] = -(farPlane + nearPlane) / (farPlane - nearPlane);	
				
			return result;	
		}	

		static Matrix4<T, Base> Orthographic(const Frustum& frustum)
		{
			return Orthographic((T)frustum.Left, (T)frustum.Right, (T)frustum.Bottom, (T)frustum.Top, (T)frustum.Near, (T)frustum.Far);
		}
				
		static Matrix4<T, Base> Perspective(T fovy, T aspect, T nearPlane, T farPlane)
		{	
			Matrix4<T, Base> result = Identity();	
				
			T tanHalfFov = tan(fovy / 2.0f);

			result.Element(0, 0) = 1 / (aspect * tanHalfFov);
			result.Element(1, 1) = 1 / (tanHalfFov);
			result.Element(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
			result.Element(3, 3) = 0;
			result.Element(3, 2) = -1;
			result.Element(2, 3) = -(2 * farPlane * nearPlane) / (farPlane - nearPlane);
				
			return result;	
		}	

		static Matrix4<T, Base> Perspective(const Frustum& frustum)
		{
			float width = frustum.Right - frustum.Left;
			float height = frustum.Top - frustum.Bottom;
			float aspect = width / height;
			float fov = atan(height / frustum.Near);
			return Perspective(fov, aspect, frustum.Near, frustum.Far);
		}
				
		static Matrix4<T, Base> Translation(const Vector4<T>& translation)
		{	
			Matrix4<T, Base> result = Identity();	
			result.SetColumn(3, translation);	
			return result;	
		}	
				
		static Matrix4<T, Base> Translation(const Vector3<T>& translation)
		{	
			return Translation(Vector4<T>(translation, (T)1));	
		}	
				
		static Matrix4<T, Base> Translation(T x, T y, T z)
		{	
			return Translation(Vector3<T>(x, y, z));
		}	
				
		static Matrix4<T, Base> Translation(const Vector2<T>& translation)
		{	
			return Translation(Vector3<T>(translation, (T)0));
		}	
				
		static Matrix4<T, Base> Rotation(T angle, const Vector4<T>& axis)
		{	
			return Rotation(angle, axis.xyz());	
		}	
				
		static Matrix4<T, Base> Rotation(T angle, const Vector3<T>& axis)
		{	
			Matrix4<T, Base> result = Identity();	
				
			T c = (T)cos(angle);	
			T s = (T)sin(angle);	
			T omc = (T)1 - c;	
				
			T x = axis.x;	
			T y = axis.y;	
			T z = axis.z;	
				
			result.values[0 + 0 * 4] = x * omc + c;	
			result.values[1 + 0 * 4] = y * x * omc + z * s;	
			result.values[2 + 0 * 4] = x * z * omc - y * s;	
				
			result.values[0 + 1 * 4] = x * y * omc - z * s;	
			result.values[1 + 1 * 4] = y * omc + c;	
			result.values[2 + 1 * 4] = y * z * omc + x * s;	
				
			result.values[0 + 2 * 4] = x * z * omc + y * s;	
			result.values[1 + 2 * 4] = y * z * omc - x * s;	
			result.values[2 + 2 * 4] = z * omc + c;	
				
			return result;	
		}	
				
		static Matrix4<T, Base> Scale(const Vector4<T>& scale)
		{	
			return Matrix4<T, Base>(scale);	
		}	
				
		static Matrix4<T, Base> Scale(const Vector3<T>& scale)
		{	
			return Scale(Vector4<T>(scale, (T)1));	
		}	
				
		static Matrix4<T, Base> Scale(T x, T y, T z)
		{	
			return Scale(Vector3<T>(x, y, z));	
		}	
				
		static Matrix4<T, Base> Scale(const Vector2<T>& scale)
		{	
			return Scale(Vector3<T>(scale, (T)1));
		}	
				
		static Matrix4<T, Base> Scale(T scale)
		{	
			return Scale(Vector3<T>(scale, scale, scale));	
		}	
				
		static Matrix4<T, Base> InvertX(const Matrix4<T, Base>& mat)
		{	
			Matrix4<T, Base> flipMatrix = Scale((T)-1, (T)1, (T)1);	
			return flipMatrix * mat * flipMatrix;	
		}	
				
		static Matrix4<T, Base> InvertY(const Matrix4<T, Base>& mat)
		{	
			Matrix4<T, Base> flipMatrix = Scale((T)1, (T)-1, (T)1);	
			return flipMatrix * mat * flipMatrix;	
		}	
				
		static Matrix4<T, Base> InvertZ(const Matrix4<T, Base>& mat)	
		{	
			Matrix4<T, Base> flipMatrix = Scale((T)1, (T)1, (T)-1);	
			return flipMatrix * mat * flipMatrix;	
		}

	};

	typedef Matrix4<float> Matrix4f;
	typedef Matrix4<int> Matrix4i;

}