#pragma once
#include "../../Components/Transform.h"
#include "Graphics/Resources/Meshes/Mesh.h"

namespace Bolt
{

	class UIElement;

	class BLT_API UIFactory
	{
	private:
		UIElement& m_ParentElement;

	public:
		UIFactory(UIElement& parentElement);

		UIElement& CreateElement(Transform transform = Transform());
	};

}