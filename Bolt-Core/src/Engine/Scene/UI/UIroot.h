#pragma once
#include "UIelement.h"

namespace Bolt
{

	class BLT_API UIroot : public UIelement
	{
	private:
		Layer* m_Layer;

	public:
		UIroot(Layer* layer, Transform&& anchorPosition = Transform());
		UIroot(Layer* layer, GameObject* object);

		Layer* GetLayer() const;

		void SetAnchorTransform(Transform&& anchorPosition);

	};

}