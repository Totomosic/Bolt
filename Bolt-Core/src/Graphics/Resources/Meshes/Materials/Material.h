#pragma once
#include "Shaders/ShaderLinkContext.h"
#include "RenderSettings.h"
#include "Graphics/Resources/ResourcePtr.h"
#include "MaterialBuilder.h"

namespace Bolt
{

	class BLT_API Material
	{
	private:
		MaterialBuilder m_Builder;
		ShaderLinkContext m_Shader;
		RenderSettings m_RenderSettings;

	private:
		Material(const MaterialBuilder& builder, ShaderLinkContext&& shader);

	public:
		Material(const Material& other) = delete;
		Material& operator=(const Material& other) = delete;
		Material(Material&& other) = default;
		Material& operator=(Material&& other) = default;
		~Material() = default;

		const ShaderLinkContext& GetShader() const;
		ShaderLinkContext& GetShader();
		const RenderSettings& GetRenderSettings() const;
		RenderSettings& GetRenderSettings();

		MaterialBuilder& GetBuilder();
		void Rebuild();

		std::unique_ptr<Material> Clone() const;

		friend class MaterialBuilder;

	};

}