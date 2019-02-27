#include "Types.h"
#include "PositionOffsetModule.h"

namespace Bolt
{

	PositionOffsetModule::PositionOffsetModule(ShaderValuePtr positionOffset) : MaterialModule(ShaderType::Vertex),
		m_PositionOffset(positionOffset), m_WorldPosition(), m_ViewPosition(), m_ScreenPosition()
	{
	
	}

	void PositionOffsetModule::AddInstructions(ShaderFactory& factory)
	{
		factory.CurrentShader(ShaderType::Vertex);
		ShaderValuePtr modelMatrix = factory.RendererUniform(RendererUniform::ModelMatrix);
		ShaderValuePtr viewMatrix = factory.RendererUniform(RendererUniform::ViewMatrix);
		ShaderValuePtr projectionMatrix = factory.RendererUniform(RendererUniform::ProjectionMatrix);
		ShaderValuePtr position = factory.GetStream(ShaderStream::Position);
		ShaderValuePtr worldPos = factory.FuncResult(factory.Operations().Mul(), { modelMatrix, position });
		m_WorldPosition = factory.FuncResult(factory.Operations().Add(), { worldPos, factory.FuncResult(factory.Operations().Vec4(), { GetValue(factory, m_PositionOffset), ShaderValue::Literal(0.0f) }) });
		m_ViewPosition = factory.FuncResult(factory.Operations().Mul(), { viewMatrix, m_WorldPosition });
		m_ScreenPosition = factory.FuncResult(factory.Operations().Mul(), { projectionMatrix, m_ViewPosition });
		factory.SetAttribute(ShaderAttribute::Position, m_ScreenPosition);
	}

	const ShaderValuePtr& PositionOffsetModule::PositionOffset() const
	{
		return m_PositionOffset;
	}

	const ShaderValuePtr& PositionOffsetModule::WorldPosition() const
	{
		return m_WorldPosition;
	}

	const ShaderValuePtr& PositionOffsetModule::ViewPosition() const
	{
		return m_ViewPosition;
	}

	const ShaderValuePtr& PositionOffsetModule::ScreenPosition() const
	{
		return m_ScreenPosition;
	}

}