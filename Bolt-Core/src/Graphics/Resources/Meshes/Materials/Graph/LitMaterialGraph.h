#pragma once
#include "Nodes/MaterialNode.h"

namespace Bolt
{

	// Material Graph that represents a material lit using phong shading
	class BLT_API LitMaterialGraph
	{
	private:
		
	public:
		LitMaterialGraph();

		// Sets the world position of vertex - defaults to MVP * in_Position
		void SetPosition(const ShaderValuePtr& value);
		// Sets the default color of fragment - defaults to White
		void SetBaseColor(const ShaderValuePtr& value);
		// Sets the normal of the vertex in tangent space - defaults to (0, 1, 0)
		void SetNormal(const ShaderValuePtr& value);
		// Sets the alpha value of the fragment - defaults to 1.0f
		void SetAlpha(const ShaderValuePtr& value);
		// Sets the threshold for the alpha value at which fragment will be discarded - defaults to 0.0f
		void SetAlphaThreshold(const ShaderValuePtr& value);

	};

}