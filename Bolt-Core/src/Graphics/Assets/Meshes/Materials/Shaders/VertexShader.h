#pragma once
#include "ShaderProgram.h"
#include "Graphics/Buffers/BufferLayout.h"

namespace Bolt
{

	class BLT_API VertexShader : public ShaderProgram
	{
	private:
		BufferLayout m_Layout;
		std::unordered_map<int, ShaderVariablePtr> m_Streams;

		ShaderVariablePtr m_VertexPosition;

	public:
		VertexShader(const BufferLayout& layout = BufferLayout::Default());

		const BufferLayout& GetLayout() const;
		void SetBufferLayout(const BufferLayout& layout);

		const ShaderVariablePtr& Stream(int streamIndex) const;
		void SetVertexPosition(const ShaderValuePtr& value);

		CompiledShaderProgram Compile() const override;
		void Reset() override;

	protected:
		ShaderVariablePtr PrivateStream(int streamIndex) override;
		ValueType GetTypeOfShaderStream(int streamIndex) const;
		void CreateStreams();

	};

}