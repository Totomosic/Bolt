#pragma once
#include "..\MaterialModule.h"

namespace Bolt
{

	class BLT_API PositionOffsetModule : public MaterialModule
	{
	private:
		ShaderValuePtr m_PositionOffset;

		ShaderValuePtr m_WorldPosition;
		ShaderValuePtr m_ViewPosition;
		ShaderValuePtr m_ScreenPosition;

	public:
		PositionOffsetModule(ShaderValuePtr positionOffset = ShaderValue::Literal(Vector3f()));

		void AddInstructions(ShaderFactory& factory) override;

		const ShaderValuePtr& PositionOffset() const;
		const ShaderValuePtr& WorldPosition() const;
		const ShaderValuePtr& ViewPosition() const;
		const ShaderValuePtr& ScreenPosition() const;

	};

}