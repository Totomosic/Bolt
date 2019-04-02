#pragma once
#include "Bolt-Core.h"
#include "Shaders\__Shaders__.h"

#include "RenderSettings.h"
#include "..\..\ResourcePtr.h"

namespace Bolt
{

	class MaterialBuilder;

	class BLT_API Material
	{
	private:
		const MaterialBuilder* m_Builder;
		ShaderLinkContext m_Shader;
		RenderSettings m_RenderSettings;

	private:
		Material(ShaderLinkContext&& shader);

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

		std::unique_ptr<Material> Clone() const;

		friend class MaterialBuilder;

	private:
		void SetBuilder(const MaterialBuilder* builder);

	};

}