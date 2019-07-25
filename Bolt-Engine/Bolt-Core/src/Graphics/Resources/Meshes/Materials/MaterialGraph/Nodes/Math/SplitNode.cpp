#include "bltpch.h"
#include "SplitNode.h"
#include "../../MaterialGraph.h"
#include "../Defaults/VectorNodes.h"

namespace Bolt
{

	// SPLIT VECTOR3 NODE

	SplitVec3Node::SplitVec3Node() : MaterialNode(ShaderStageCompatibility::All, 1, 8)
	{

	}

	NodeConnection SplitVec3Node::GetR() const
	{
		return GetConnection(0);
	}

	NodeConnection SplitVec3Node::GetG() const
	{
		return GetConnection(1);
	}

	NodeConnection SplitVec3Node::GetB() const
	{
		return GetConnection(2);
	}

	void SplitVec3Node::SetInput(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void SplitVec3Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::x(inputs.GetInput(0)));
		node.BuildOutput(1, ShaderFuncs::y(inputs.GetInput(0)));
		node.BuildOutput(2, ShaderFuncs::z(inputs.GetInput(0)));
	}

	void SplitVec3Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<Vec3Node>(Vector3f(0.0f, 0.0f, 0.0f))).GetValue());
	}

	// SPLIT VECTOR4 NODE

	SplitVec4Node::SplitVec4Node() : MaterialNode(ShaderStageCompatibility::All, 1, 8)
	{

	}

	NodeConnection SplitVec4Node::GetRGB() const
	{
		return GetConnection(0);
	}

	NodeConnection SplitVec4Node::GetRG() const
	{
		return GetConnection(1);
	}

	NodeConnection SplitVec4Node::GetGB() const
	{
		return GetConnection(2);
	}

	NodeConnection SplitVec4Node::GetRB() const
	{
		return GetConnection(3);
	}

	NodeConnection SplitVec4Node::GetR() const
	{
		return GetConnection(4);
	}

	NodeConnection SplitVec4Node::GetG() const
	{
		return GetConnection(5);
	}

	NodeConnection SplitVec4Node::GetB() const
	{
		return GetConnection(6);
	}

	NodeConnection SplitVec4Node::GetA() const
	{
		return GetConnection(7);
	}

	void SplitVec4Node::SetInput(const NodeConnection& connection)
	{
		Connect(0, connection);
	}

	void SplitVec4Node::Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const
	{
		node.BuildOutput(0, ShaderFuncs::xyz(inputs.GetInput(0)));
		node.BuildOutput(1, ShaderFuncs::xy(inputs.GetInput(0)));
		node.BuildOutput(2, ShaderFuncs::yz(inputs.GetInput(0)));
		node.BuildOutput(3, ShaderFuncs::xz(inputs.GetInput(0)));
		node.BuildOutput(4, ShaderFuncs::x(inputs.GetInput(0)));
		node.BuildOutput(5, ShaderFuncs::y(inputs.GetInput(0)));
		node.BuildOutput(6, ShaderFuncs::z(inputs.GetInput(0)));
		node.BuildOutput(7, ShaderFuncs::w(inputs.GetInput(0)));
	}

	void SplitVec4Node::ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context)
	{
		Connect(0, graph.AddNode(std::make_unique<Vec4Node>(Vector4f(0.0f, 0.0f, 0.0f, 0.0f))).GetValue());
	}

}