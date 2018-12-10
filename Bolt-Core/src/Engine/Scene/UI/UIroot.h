#pragma once
#include "UIelement.h"
#include "..\ObjectFactory.h"

namespace Bolt
{

	struct Layer;

	class BLT_API UIroot : public UIelement
	{
	private:
		ObjectFactory m_Factory;

	public:
		UIroot();
		UIroot(Layer* layer, GameObject* object);
		~UIroot() override;

		friend class UIelement;
		friend struct Layer;

	protected:
		const ObjectFactory& GetFactory() const;
		ObjectFactory& GetFactory();

	};

}