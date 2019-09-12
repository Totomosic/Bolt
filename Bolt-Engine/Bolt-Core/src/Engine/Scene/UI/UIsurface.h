#pragma once
#include "UIElement.h"

namespace Bolt
{

	class BLT_API UISurface : public UIElement
	{
	public:
		UISurface(UIManager* manager, UIElement* parent);
	};

}