#pragma once
#include "ShaderUniform.h"

namespace Bolt
{

	class ShaderRendererUniform;
	using ShaderRendererUniformPtr = std::shared_ptr<ShaderRendererUniform>;

	class BLT_API ShaderRendererUniform : public ShaderUniform
	{
	private:
		RendererUniform m_Uniform;

	public:
		ShaderRendererUniform(RendererUniform uniform);

		RendererUniform UniformType() const;

	};

}