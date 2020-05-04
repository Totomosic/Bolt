#pragma once
#include "Shaders/ShaderLinkContext.h"
#include "RenderSettings.h"
#include "Graphics/Assets/AssetHandle.h"
#include "MaterialBuilder.h"

namespace Bolt
{

	class BLT_API Material
	{
	private:
		ShaderLinkContext m_Shader;
		RenderSettings m_RenderSettings;

	public:
		Material(ShaderLinkContext&& shader, bool isTransparent);
		Material(const ShaderLinkContext& shader, bool isTransparent);

		const ShaderLinkContext& GetLinkContext() const;
		ShaderLinkContext& GetLinkContext();
		const RenderSettings& GetRenderSettings() const;
		RenderSettings& GetRenderSettings();
		bool IsTransparent() const;

		void SetIsTransparent(bool isTransparent);

		template<typename MaterialT = Material>
		std::unique_ptr<MaterialT> Clone() const
		{
			std::unique_ptr<MaterialT> material = std::make_unique<MaterialT>((const MaterialT&)*this);
			return material;
		}

		friend class MaterialBuilder;

	};

}