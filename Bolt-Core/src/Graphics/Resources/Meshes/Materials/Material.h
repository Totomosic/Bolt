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
		ShaderLinkContext m_Shader;
		RenderSettings m_RenderSettings;
		bool m_IsTransparent;

	public:
		Material(ShaderLinkContext&& shader, bool isTransparent);
		Material(const ShaderLinkContext& shader, bool isTransparent);

		const ShaderLinkContext& GetShader() const;
		ShaderLinkContext& GetShader();
		const RenderSettings& GetRenderSettings() const;
		RenderSettings& GetRenderSettings();
		bool IsTransparent() const;

		void SetIsTransparent(bool isTransparent, bool updateRenderSettings = true);

		std::unique_ptr<Material> Clone() const;

		friend class MaterialBuilder;

	};

}