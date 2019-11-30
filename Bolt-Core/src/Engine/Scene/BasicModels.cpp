#include "bltpch.h"
#include "BasicModels.h"
#include "Engine/Engine.h"
#include "Graphics/Resources/ResourceManager.h"

#include "Graphics/Resources/Meshes/Factories/RectangleFactory.h"
#include "Graphics/Resources/Meshes/Factories/CuboidFactory.h"
#include "Graphics/Resources/Meshes/Factories/EllipseFactory.h"
#include "Graphics/Resources/Meshes/Factories/SphereFactory.h"

#include "Core/Profiling/Profiling.h"

namespace Bolt
{

	BasicModels& BasicModels::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetBasicModels();
	}

	BasicModels::BasicModels(ResourceManager* manager)
	{
		BLT_PROFILE_FUNCTION();
		m_SquareModel = manager->Register(std::make_unique<Model>(RectangleFactory(1, 1, Color::White)));
		m_CircleModel = manager->Register(std::make_unique<Model>(EllipseFactory(2, 2, 360, Color::White)));
		m_CubeModel = manager->Register(std::make_unique<Model>(CuboidFactory(1, 1, 1, Color::White)));
		m_SphereModel = manager->Register(std::make_unique<Model>(SphereFactory(1, 100, Color::White)));
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

	const ResourcePtr<Model>& BasicModels::Sphere() const
	{
		return m_SphereModel;
	}

}