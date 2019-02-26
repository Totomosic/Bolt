#pragma once
#include "BuiltInFunc.h"

namespace Bolt
{

#define BLT_DEFINE_BUILT_IN_FUNC(name)	\
private:	\
	BuiltInFunc m_##name;	\
public:	\
	inline const BuiltInFunc& name() const { return m_##name; }

	class BLT_API BuiltInContext
	{
	public:
		BuiltInContext();

		BLT_DEFINE_BUILT_IN_FUNC(Add);
		BLT_DEFINE_BUILT_IN_FUNC(Sub);
		BLT_DEFINE_BUILT_IN_FUNC(Mul);
		BLT_DEFINE_BUILT_IN_FUNC(Div);
		BLT_DEFINE_BUILT_IN_FUNC(Pow);

		BLT_DEFINE_BUILT_IN_FUNC(x);
		BLT_DEFINE_BUILT_IN_FUNC(y);
		BLT_DEFINE_BUILT_IN_FUNC(z);
		BLT_DEFINE_BUILT_IN_FUNC(w);
		BLT_DEFINE_BUILT_IN_FUNC(xy);
		BLT_DEFINE_BUILT_IN_FUNC(xyz);
		BLT_DEFINE_BUILT_IN_FUNC(Vec2);
		BLT_DEFINE_BUILT_IN_FUNC(Vec3);
		BLT_DEFINE_BUILT_IN_FUNC(Vec4);

		BLT_DEFINE_BUILT_IN_FUNC(Texture);

	};

}