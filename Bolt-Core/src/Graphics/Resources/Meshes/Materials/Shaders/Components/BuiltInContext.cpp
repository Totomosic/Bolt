#include "Types.h"
#include "BuiltInContext.h"

namespace Bolt
{

	BuiltInContext::BuiltInContext()
	{
		m_Add.AddOverload({ ValueType::Float, ValueType::Float }, ValueType::Float, "p0 + p1");
		m_Add.AddOverload({ ValueType::Int, ValueType::Int }, ValueType::Int, "p0 + p1");
		m_Add.AddOverload({ ValueType::Int, ValueType::Float }, ValueType::Float, "p0 + p1");
		m_Add.AddOverload({ ValueType::Float, ValueType::Int }, ValueType::Float, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector2f, ValueType::Float }, ValueType::Vector2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector2f, ValueType::Int }, ValueType::Vector2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector2f, ValueType::Vector2f }, ValueType::Vector2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector2f, ValueType::Vector2i }, ValueType::Vector2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Float, ValueType::Vector2f }, ValueType::Vector2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Int, ValueType::Vector2f }, ValueType::Vector2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector2i, ValueType::Vector2f }, ValueType::Vector2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector3f, ValueType::Float }, ValueType::Vector3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector3f, ValueType::Int }, ValueType::Vector3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector3f, ValueType::Vector3f }, ValueType::Vector3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector3f, ValueType::Vector3i }, ValueType::Vector3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Float, ValueType::Vector3f }, ValueType::Vector3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Int, ValueType::Vector3f }, ValueType::Vector3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector3i, ValueType::Vector3f }, ValueType::Vector3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector4f, ValueType::Float }, ValueType::Vector4f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector4f, ValueType::Int }, ValueType::Vector4f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector4f, ValueType::Vector4f }, ValueType::Vector4f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector4f, ValueType::Vector4i }, ValueType::Vector4f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Float, ValueType::Vector4f }, ValueType::Vector4f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Int, ValueType::Vector4f }, ValueType::Vector4f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Vector4i, ValueType::Vector4f }, ValueType::Vector4f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Matrix2f, ValueType::Float }, ValueType::Matrix2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Matrix2f, ValueType::Int }, ValueType::Matrix2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Matrix2f, ValueType::Matrix2f }, ValueType::Matrix2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Float, ValueType::Matrix2f }, ValueType::Matrix2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Int, ValueType::Matrix2f }, ValueType::Matrix2f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Matrix3f, ValueType::Matrix3f }, ValueType::Matrix3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Float, ValueType::Matrix3f }, ValueType::Matrix3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Int, ValueType::Matrix3f }, ValueType::Matrix3f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Matrix4f, ValueType::Matrix4f }, ValueType::Matrix4f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Float, ValueType::Matrix4f }, ValueType::Matrix4f, "p0 + p1");
		m_Add.AddOverload({ ValueType::Int, ValueType::Matrix4f }, ValueType::Matrix4f, "p0 + p1");

		m_Sub.AddOverload({ ValueType::Float, ValueType::Float }, ValueType::Float, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Int, ValueType::Int }, ValueType::Int, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Int, ValueType::Float }, ValueType::Float, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Float, ValueType::Int }, ValueType::Float, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector2f, ValueType::Float }, ValueType::Vector2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector2f, ValueType::Int }, ValueType::Vector2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector2f, ValueType::Vector2f }, ValueType::Vector2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector2f, ValueType::Vector2i }, ValueType::Vector2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Float, ValueType::Vector2f }, ValueType::Vector2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Int, ValueType::Vector2f }, ValueType::Vector2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector2i, ValueType::Vector2f }, ValueType::Vector2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector3f, ValueType::Float }, ValueType::Vector3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector3f, ValueType::Int }, ValueType::Vector3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector3f, ValueType::Vector3f }, ValueType::Vector3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector3f, ValueType::Vector3i }, ValueType::Vector3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Float, ValueType::Vector3f }, ValueType::Vector3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Int, ValueType::Vector3f }, ValueType::Vector3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector3i, ValueType::Vector3f }, ValueType::Vector3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector4f, ValueType::Float }, ValueType::Vector4f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector4f, ValueType::Int }, ValueType::Vector4f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector4f, ValueType::Vector4f }, ValueType::Vector4f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector4f, ValueType::Vector4i }, ValueType::Vector4f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Float, ValueType::Vector4f }, ValueType::Vector4f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Int, ValueType::Vector4f }, ValueType::Vector4f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Vector4i, ValueType::Vector4f }, ValueType::Vector4f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Matrix2f, ValueType::Float }, ValueType::Matrix2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Matrix2f, ValueType::Int }, ValueType::Matrix2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Matrix2f, ValueType::Matrix2f }, ValueType::Matrix2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Float, ValueType::Matrix2f }, ValueType::Matrix2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Int, ValueType::Matrix2f }, ValueType::Matrix2f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Matrix3f, ValueType::Matrix3f }, ValueType::Matrix3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Float, ValueType::Matrix3f }, ValueType::Matrix3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Int, ValueType::Matrix3f }, ValueType::Matrix3f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Matrix4f, ValueType::Matrix4f }, ValueType::Matrix4f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Float, ValueType::Matrix4f }, ValueType::Matrix4f, "p0 - p1");
		m_Sub.AddOverload({ ValueType::Int, ValueType::Matrix4f }, ValueType::Matrix4f, "p0 - p1");

		m_Mul.AddOverload({ ValueType::Float, ValueType::Float }, ValueType::Float, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Int, ValueType::Int }, ValueType::Int, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Int, ValueType::Float }, ValueType::Float, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Float, ValueType::Int }, ValueType::Float, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector2f, ValueType::Float }, ValueType::Vector2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector2f, ValueType::Int }, ValueType::Vector2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector2f, ValueType::Vector2f }, ValueType::Vector2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector2f, ValueType::Vector2i }, ValueType::Vector2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Float, ValueType::Vector2f }, ValueType::Vector2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Int, ValueType::Vector2f }, ValueType::Vector2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector2i, ValueType::Vector2f }, ValueType::Vector2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector3f, ValueType::Float }, ValueType::Vector3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector3f, ValueType::Int }, ValueType::Vector3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector3f, ValueType::Vector3f }, ValueType::Vector3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector3f, ValueType::Vector3i }, ValueType::Vector3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Float, ValueType::Vector3f }, ValueType::Vector3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Int, ValueType::Vector3f }, ValueType::Vector3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector3i, ValueType::Vector3f }, ValueType::Vector3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector4f, ValueType::Float }, ValueType::Vector4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector4f, ValueType::Int }, ValueType::Vector4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector4f, ValueType::Vector4f }, ValueType::Vector4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector4f, ValueType::Vector4i }, ValueType::Vector4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Float, ValueType::Vector4f }, ValueType::Vector4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Int, ValueType::Vector4f }, ValueType::Vector4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Vector4i, ValueType::Vector4f }, ValueType::Vector4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Matrix2f, ValueType::Float }, ValueType::Matrix2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Matrix2f, ValueType::Int }, ValueType::Matrix2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Matrix2f, ValueType::Matrix2f }, ValueType::Matrix2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Float, ValueType::Matrix2f }, ValueType::Matrix2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Int, ValueType::Matrix2f }, ValueType::Matrix2f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Matrix3f, ValueType::Matrix3f }, ValueType::Matrix3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Float, ValueType::Matrix3f }, ValueType::Matrix3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Int, ValueType::Matrix3f }, ValueType::Matrix3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Matrix4f, ValueType::Matrix4f }, ValueType::Matrix4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Float, ValueType::Matrix4f }, ValueType::Matrix4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Int, ValueType::Matrix4f }, ValueType::Matrix4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Matrix4f, ValueType::Vector4f }, ValueType::Vector4f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Matrix3f, ValueType::Vector3f }, ValueType::Vector3f, "p0 * p1");
		m_Mul.AddOverload({ ValueType::Matrix2f, ValueType::Vector2f }, ValueType::Vector2f, "p0 * p1");

		m_Div.AddOverload({ ValueType::Float, ValueType::Float }, ValueType::Float, "p0 / p1");
		m_Div.AddOverload({ ValueType::Int, ValueType::Int }, ValueType::Int, "p0 / p1");
		m_Div.AddOverload({ ValueType::Int, ValueType::Float }, ValueType::Float, "p0 / p1");
		m_Div.AddOverload({ ValueType::Float, ValueType::Int }, ValueType::Float, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector2f, ValueType::Float }, ValueType::Vector2f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector2f, ValueType::Int }, ValueType::Vector2f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector2f, ValueType::Vector2f }, ValueType::Vector2f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector2f, ValueType::Vector2i }, ValueType::Vector2f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Float, ValueType::Vector2f }, ValueType::Vector2f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Int, ValueType::Vector2f }, ValueType::Vector2f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector2i, ValueType::Vector2f }, ValueType::Vector2f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector3f, ValueType::Float }, ValueType::Vector3f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector3f, ValueType::Int }, ValueType::Vector3f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector3f, ValueType::Vector3f }, ValueType::Vector3f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector3f, ValueType::Vector3i }, ValueType::Vector3f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Float, ValueType::Vector3f }, ValueType::Vector3f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Int, ValueType::Vector3f }, ValueType::Vector3f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector3i, ValueType::Vector3f }, ValueType::Vector3f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector4f, ValueType::Float }, ValueType::Vector4f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector4f, ValueType::Int }, ValueType::Vector4f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector4f, ValueType::Vector4f }, ValueType::Vector4f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Vector4f, ValueType::Vector4i }, ValueType::Vector4f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Float, ValueType::Vector4f }, ValueType::Vector4f, "p0 / p1");
		m_Div.AddOverload({ ValueType::Int, ValueType::Vector4f }, ValueType::Vector4f, "p0 / p1");

		m_Pow.AddOverload({ ValueType::Int, ValueType::Int }, ValueType::Int, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Float, ValueType::Int }, ValueType::Float, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Float, ValueType::Float }, ValueType::Float, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector2i, ValueType::Int }, ValueType::Vector2i, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector2f, ValueType::Int }, ValueType::Vector2f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector3i, ValueType::Int }, ValueType::Vector3i, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector3f, ValueType::Int }, ValueType::Vector3f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector4i, ValueType::Int }, ValueType::Vector4i, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector4f, ValueType::Int }, ValueType::Vector4f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector2i, ValueType::Float }, ValueType::Vector2f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector2f, ValueType::Float }, ValueType::Vector2f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector3i, ValueType::Float }, ValueType::Vector3f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector3f, ValueType::Float }, ValueType::Vector3f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector4i, ValueType::Float }, ValueType::Vector4f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector4f, ValueType::Float }, ValueType::Vector4f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector2i, ValueType::Vector2i }, ValueType::Vector2i, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector2f, ValueType::Vector2i }, ValueType::Vector2f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector3i, ValueType::Vector3i }, ValueType::Vector3i, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector3f, ValueType::Vector3i }, ValueType::Vector3f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector4i, ValueType::Vector4i }, ValueType::Vector4i, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector4f, ValueType::Vector4i }, ValueType::Vector4f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector2i, ValueType::Vector2f }, ValueType::Vector2f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector2f, ValueType::Vector2f }, ValueType::Vector2f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector3i, ValueType::Vector3f }, ValueType::Vector3f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector3f, ValueType::Vector3f }, ValueType::Vector3f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector4i, ValueType::Vector4f }, ValueType::Vector4f, "pow(p0, p1)");
		m_Pow.AddOverload({ ValueType::Vector4f, ValueType::Vector4f }, ValueType::Vector4f, "pow(p0, p1)");

		m_x.AddOverload({ ValueType::Vector2i }, ValueType::Int, "p0.x");
		m_x.AddOverload({ ValueType::Vector2f }, ValueType::Float, "p0.x");
		m_x.AddOverload({ ValueType::Vector3i }, ValueType::Int, "p0.x");
		m_x.AddOverload({ ValueType::Vector3f }, ValueType::Float, "p0.x");
		m_x.AddOverload({ ValueType::Vector4i }, ValueType::Int, "p0.x");
		m_x.AddOverload({ ValueType::Vector4f }, ValueType::Float, "p0.x");

		m_y.AddOverload({ ValueType::Vector2i }, ValueType::Int, "p0.y");
		m_y.AddOverload({ ValueType::Vector2f }, ValueType::Float, "p0.y");
		m_y.AddOverload({ ValueType::Vector3i }, ValueType::Int, "p0.y");
		m_y.AddOverload({ ValueType::Vector3f }, ValueType::Float, "p0.y");
		m_y.AddOverload({ ValueType::Vector4i }, ValueType::Int, "p0.y");
		m_y.AddOverload({ ValueType::Vector4f }, ValueType::Float, "p0.y");

		m_z.AddOverload({ ValueType::Vector3i }, ValueType::Int, "p0.z");
		m_z.AddOverload({ ValueType::Vector3f }, ValueType::Float, "p0.z");
		m_z.AddOverload({ ValueType::Vector4i }, ValueType::Int, "p0.z");
		m_z.AddOverload({ ValueType::Vector4f }, ValueType::Float, "p0.z");

		m_w.AddOverload({ ValueType::Vector4i }, ValueType::Int, "p0.w");
		m_w.AddOverload({ ValueType::Vector4f }, ValueType::Float, "p0.w");

		m_xy.AddOverload({ ValueType::Vector3i }, ValueType::Vector2i, "p0.xy");
		m_xy.AddOverload({ ValueType::Vector3f }, ValueType::Vector2f, "p0.xy");
		m_xy.AddOverload({ ValueType::Vector4i }, ValueType::Vector2i, "p0.xy");
		m_xy.AddOverload({ ValueType::Vector4f }, ValueType::Vector2f, "p0.xy");

		m_xyz.AddOverload({ ValueType::Vector4i }, ValueType::Vector3i, "p0.xyz");
		m_xyz.AddOverload({ ValueType::Vector4f }, ValueType::Vector3f, "p0.xyz");

		m_Vec2.AddOverload({ ValueType::Int }, ValueType::Vector2i, "vec2(p0)");
		m_Vec2.AddOverload({ ValueType::Float }, ValueType::Vector2f, "vec2(p0)");
		m_Vec2.AddOverload({ ValueType::Int, ValueType::Int }, ValueType::Vector2i, "vec2(p0, p1)");
		m_Vec2.AddOverload({ ValueType::Float, ValueType::Float }, ValueType::Vector2f, "vec2(p0, p1)");

		m_Vec3.AddOverload({ ValueType::Int }, ValueType::Vector3i, "vec3(p0)");
		m_Vec3.AddOverload({ ValueType::Float }, ValueType::Vector3f, "vec3(p0)");
		m_Vec3.AddOverload({ ValueType::Int, ValueType::Int, ValueType::Int }, ValueType::Vector3i, "vec3(p0, p1, p2)");
		m_Vec3.AddOverload({ ValueType::Float, ValueType::Float, ValueType::Float }, ValueType::Vector3f, "vec3(p0, p1, p2)");
		m_Vec3.AddOverload({ ValueType::Vector2i, ValueType::Int }, ValueType::Vector3i, "vec3(p0, p1)");
		m_Vec3.AddOverload({ ValueType::Vector2f, ValueType::Float }, ValueType::Vector3f, "vec3(p0, p1)");

		m_Vec4.AddOverload({ ValueType::Int }, ValueType::Vector4i, "vec4(p0)");
		m_Vec4.AddOverload({ ValueType::Float }, ValueType::Vector4f, "vec4(p0)");
		m_Vec4.AddOverload({ ValueType::Int, ValueType::Int, ValueType::Int, ValueType::Int }, ValueType::Vector4i, "vec4(p0, p1, p2, p3)");
		m_Vec4.AddOverload({ ValueType::Float, ValueType::Float, ValueType::Float, ValueType::Float }, ValueType::Vector4f, "vec4(p0, p1, p2, p3)");
		m_Vec4.AddOverload({ ValueType::Vector2i, ValueType::Int, ValueType::Int }, ValueType::Vector4i, "vec4(p0, p1, p2)");
		m_Vec4.AddOverload({ ValueType::Vector2f, ValueType::Float, ValueType::Float }, ValueType::Vector4f, "vec4(p0, p1, p2)");
		m_Vec4.AddOverload({ ValueType::Vector3i, ValueType::Int }, ValueType::Vector4i, "vec4(p0, p1)");
		m_Vec4.AddOverload({ ValueType::Vector3f, ValueType::Float }, ValueType::Vector4f, "vec4(p0, p1)");

		m_Texture.AddOverload({ ValueType::Texture1D, ValueType::Float }, ValueType::Vector4f, "texture(p0, p1)");
		m_Texture.AddOverload({ ValueType::Texture2D, ValueType::Vector2f }, ValueType::Vector4f, "texture(p0, p1)");
		m_Texture.AddOverload({ ValueType::Texture3D, ValueType::Vector3f }, ValueType::Vector4f, "texture(p0, p1)");
		m_Texture.AddOverload({ ValueType::TextureCube, ValueType::Vector3f }, ValueType::Vector4f, "texture(p0, p1)");

	}

}