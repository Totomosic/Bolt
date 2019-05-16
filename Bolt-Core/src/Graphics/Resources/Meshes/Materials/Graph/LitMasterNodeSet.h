#pragma once
#include "Nodes/MasterNode.h"
#include "../MaterialBuilder.h"

namespace Bolt
{

	class BLT_API LitMasterNodeSet
	{
	private:
		MaterialBuilder m_Builder;
		MasterNode m_WorldPosition;
		MasterNode m_BaseColor;
		MasterNode m_WorldNormal;
		MasterNode m_Alpha;
		MasterNode m_AlphaThreshold;

	public:
		LitMasterNodeSet();

		// Sets the world position of vertex - defaults to MVP * in_Position
		void SetPosition(const MaterialValue& value);
		// Sets the default color of fragment - defaults to White
		void SetBaseColor(const MaterialValue& value);
		// Sets the normal of the vertex in tangent space - defaults to (0, 1, 0)
		void SetNormal(const MaterialValue& value);
		// Sets the alpha value of the fragment - defaults to 1.0f
		void SetAlpha(const MaterialValue& value);
		// Sets the threshold for the alpha value at which fragment will be discarded - defaults to 0.0f
		void SetAlphaThreshold(const MaterialValue& value);

	};

}