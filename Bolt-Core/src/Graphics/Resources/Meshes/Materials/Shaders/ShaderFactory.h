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

	public:
		ShaderFactory();

		VertexShader& Vertex();
		FragmentShader& Fragment();

		ShaderLinkContext BuildShader() const;

	};

}