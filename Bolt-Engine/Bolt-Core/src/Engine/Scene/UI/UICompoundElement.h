#pragma once
#include "UIElement.h"

namespace Bolt
{

	class BLT_API UICompoundElement : public UIElement
	{
	public:
		UICompoundElement(UIManager* manager, UIElement* parent);
	};

}