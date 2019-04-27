#pragma once
#include "ShaderLinkContext.h"
#include "VertexShader.h"
#include "FragmentShader.h"

namespace Bolt
{

	class BLT_API ShaderFactory
	{
	private:
		VertexShader m_Vertex;
		FragmentShader m_Fragment;
		mutable std::shared_ptr<ShaderInstance> m_CurrentShaderInstance;
		mutable bool m_IsValid;

	public:
		ShaderFactory();

		VertexShader& Vertex();
		FragmentShader& Fragment();

		void ForceRecompile();
		ShaderLinkContext BuildShader() const;

	};

}