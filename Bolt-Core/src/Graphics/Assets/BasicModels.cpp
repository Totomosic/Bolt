#include "bltpch.h"
#include "BasicModels.h"
#include "Engine/Engine.h"
#include "Graphics/Assets/AssetManager.h"

#include "Graphics/Assets/Meshes/Factories/RectangleFactory.h"
#include "Graphics/Assets/Meshes/Factories/CuboidFactory.h"
#include "Graphics/Assets/Meshes/Factories/EllipseFactory.h"
#include "Graphics/Assets/Meshes/Factories/SphereFactory.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	BasicModels::BasicModels(AssetManager* manager)
	{
		BLT_PROFILE_FUNCTION();
		m_SquareModel = manager->Register(std::make_unique<Model>(RectangleFactory(1, 1, Color::White)));
		m_CircleModel = manager->Register(std::make_unique<Model>(EllipseFactory(2, 2, 360, Color::White)));
		m_CubeModel = manager->Register(std::make_unique<Model>(CuboidFactory(1, 1, 1, Color::White)));
		m_SphereModel = manager->Register(std::make_unique<Model>(SphereFactory(1, 100, Color::White)));
	}

	const AssetHandle<Model>& BasicModels::Square() const
	{
		return m_SquareModel;
	}

	const AssetHandle<Model>& BasicModels::Circle() const
	{
		return m_CircleModel;
	}

	const AssetHandle<Model>& BasicModels::Cube() const
	{
		return m_CubeModel;
	}

	const AssetHandle<Model>& BasicModels::Sphere() const
	{
		return m_SphereModel;
	}

}