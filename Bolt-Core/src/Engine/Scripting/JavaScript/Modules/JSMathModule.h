#pragma once
#include "../JSModule.h"

namespace Bolt
{

	class BLT_API JSMathModule : public JSModule
	{
	public:
		void AddToContext(JSContext& context) const override;
	};

}