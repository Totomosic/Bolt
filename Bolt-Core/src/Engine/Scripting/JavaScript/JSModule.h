#pragma once
#include "dukglue/dukglue.h"
#include "JSContext.h"

namespace Bolt
{

	class BLT_API JSModule
	{
	public:
		virtual void AddToContext(JSContext& context) const = 0;
	};

}