#include "Types.h"
#include "BasicModels.h"
#include "Engine/Engine.h"
#include "Graphics/Resources/ResourceManager.h"

#include "Graphics/Resources/Meshes/Factories/RectangleFactory.h"
#include "Graphics/Resources/Meshes/Factories/CuboidFactory.h"
#include "Graphics/Resources/Meshes/Factories/EllipseFactory.h"

namespace Bolt
{

	BasicModels& BasicModels::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetBasicModels();
	}

	BasicModels::BasicModels()
	{
		m_SquareModel = ResourceManager::Get().GetResource<Model>(ResourceManager::Get().Register(std::make_unique<Model>(RectangleFactory(1, 1, Color::White))));
		m_CircleModel = ResourceManager::Get().GetResource<Model>(ResourceManager::Get().Register(std::make_unique<Model>(EllipseFactory(2, 2, 360, Color::White))));
		m_CubeModel = ResourceManager::Get().GetResource<Model>(ResourceManager::Get().Register(std::make_unique<Model>(CuboidFactory(1, 1, 1, Color::White))));
	}

	const ResourcePtr<Model>& BasicModels::Square() const
	{
		return m_SquareModel;
	}

	const ResourcePtr<Model>& BasicModels::Circle() const
	{
		return m_CircleModel;
	}

	const ResourcePtr<Model>& BasicModels::Cube() const
	{
		return m_CubeModel;
	}

}