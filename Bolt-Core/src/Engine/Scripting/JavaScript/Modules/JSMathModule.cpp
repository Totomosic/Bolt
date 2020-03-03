#include "bltpch.h"
#include "JSMathModule.h"

#include "BoltLib/Math/Maths.h"

namespace Bolt
{

	void JSMathModule::AddToContext(JSContext& context) const
	{
		duk_context* ctx = context.GetNativeContext();
		dukglue_register_constructor<Vector2f>(ctx, "Vector2f");
	}

}
